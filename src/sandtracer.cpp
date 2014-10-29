#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
//#include <Windows.h>

#define IMGW 640
#define IMGH 360

using namespace std;

struct Sphere {
	// Position
	double x;
	double y;
	double z;

	// Radius
	double radius;
};

struct Triangle {
	// P1
	vector<double> v0;
	// P2
	vector<double> v1;
	// P3
	vector<double> v2;
	// normal
	vector<double> normal;
};

struct Camera {
	// Position
	vector<double> pos;

	// Sensor
	// distance from camera center
	double d;
	// size
	double w;
	double h;
	// resolution
	int pw;
	int ph;
};

vector<double> operator+(vector<double> a, vector<double> b) {
	vector<double> c(3,0);
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
	return c;
}

vector<double> operator-(vector<double> a, vector<double> b) {
	vector<double> c(3,0);
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
	return c;
}

vector<double> operator*(double a, vector<double> b) {
	vector<double> c(3,0);
	c[0] = a*b[0];
	c[1] = a*b[1];
	c[2] = a*b[2];
	return c;
}

vector<double> operator/(vector<double> a, double b) {
	vector<double> c(3,0);
	c[0] = a[0] / b;
	c[1] = a[1] / b;
	c[2] = a[2] / b;
	return c;
}

double dot(vector<double> a, vector<double> b) {
	return (double) a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

vector<double> cross(vector<double> a, vector<double> b) {
	vector<double> c(a);

	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];

	return c;
}

double norm(vector<double> const &a) {
	double res=0.0;
	for (int i = 0; i < a.size(); i++)
		res += a[i] * a[i];

	return sqrt(res);
}

vector<double> getNormal(Triangle t) {
	vector<double> ret(3);
	ret = cross(t.v1 - t.v0, t.v2 - t.v0);
	ret = ret / norm(ret);
	return ret;
}

void computeNormals(vector<Triangle> a) {
	for(int i = 0; i < a.size(); i++) {
		a[i].normal = getNormal(a[i]);
	}
}


int main(int argc, char *argv[])
{
	// Sphere definition
	Sphere mSphere;
	mSphere.x = 0;
	mSphere.y = 0;
	mSphere.z = -20;
	mSphere.radius = 0.5;

	// Triangles definition
	vector<Triangle> mTriangles(1);
	mTriangles[0].v0 = vector<double>(3);
	mTriangles[0].v0[0] = -1;
	mTriangles[0].v0[1] = 0;
	mTriangles[0].v0[2] = -20;
	
	mTriangles[0].v1 = vector<double>(3);
	mTriangles[0].v1[0] = 1;
	mTriangles[0].v1[1] = 0;
	mTriangles[0].v1[2] = -20;
	
	mTriangles[0].v2 = vector<double>(3);
	mTriangles[0].v2[0] = 0;
	mTriangles[0].v2[1] = 1;
	mTriangles[0].v2[2] = -20;

	mTriangles[0].normal = vector<double>(3);
	mTriangles[0].normal[0] = 0.0;
	mTriangles[0].normal[1] = 0.0;
	mTriangles[0].normal[2] = 1.0;


	// Camera definition
	Camera mCamera;
	mCamera.pos = vector<double>(3);
	mCamera.pos[0] = 0;
	mCamera.pos[1] = 0;
	mCamera.pos[2] = 0;
	
	mCamera.d = 0.07;
	mCamera.w = 0.0017777;
	mCamera.h = 0.001;
	mCamera.pw = IMGW;
	mCamera.ph = IMGH;

	// Rendered image
	cout << "init" << endl;
	vector<vector<int> > render(IMGW, vector<int>(IMGH, 0));
	vector<vector<vector<int> > > debugColor(IMGW, vector<vector<int> >(IMGH, vector<int>(3,0)));
	int counter = 0;

	// Pixel pos
	vector<double> ba(3, 0), dir;
	double normdir;

	// cam to sphere vector
	ba[0] = mSphere.x - mCamera.pos[0];
	ba[1] = mSphere.y - mCamera.pos[1];
	ba[2] = mSphere.z - mCamera.pos[2];

	// For each pixel try something cool
	cout << "render parallel" << endl;
	int renderTime = clock();
	#pragma omp parallel for private(dir,normdir) shared(mSphere, mTriangles, mCamera,debugColor,render)
	for (int i = 0; i < IMGW; i++){
		for (int j = 0; j < IMGH; j++){
			dir.resize(3);

			// line direction
			dir[0] = (double) - i*(mCamera.w / mCamera.pw) + mCamera.w / 2.0;
			dir[1] = (double) - j*(mCamera.h / mCamera.ph) + mCamera.h / 2.0;
			dir[2] = (double) - mCamera.d;

			// debug color
			normdir = norm(dir);
			debugColor[i][j][0] = (((dir[0]/normdir)+1.0)/2.0)*255;
			debugColor[i][j][1] = (((dir[1]/normdir)+1.0)/2.0)*255;
			debugColor[i][j][2] = (((dir[2]/normdir)+1.0)/2.0)*255;

			if (norm(cross(ba, dir)) / normdir < mSphere.radius){
				render[i][j] = (int)255.0 ;//* (1-distToSphere / mSphere.radius);
			}
			else {
				render[i][j] = 0;
			}
			dir = dir / normdir;
			for(int t = 0; t < mTriangles.size(); t++) {
				double ri = dot(mTriangles[t].normal, mTriangles[t].v0 - mCamera.pos) / dot(mTriangles[t].normal, dir);
				debugColor[i][j][0] = 0;
				debugColor[i][j][1] = 255;
				debugColor[i][j][2] = 0;
				/*#pragma omp critical
				{
				cout << "n " << mTriangles[t].normal[0] << " " << mTriangles[t].normal[1] << " " << mTriangles[t].normal[2] << " " << endl;
				cout << "d " << dir[0] << " " << dir[1] << " " << dir[2] << endl;
				cout << "l " << dot(mTriangles[t].normal, mTriangles[t].v0 - mCamera.pos) << " " << dot(mTriangles[t].normal, dir)*1000.0 << " " << ri << endl;
				}*/
				if( ri >= 0.0 && ri <= 1.0 ) {
					vector<double> pi = mTriangles[t].v0 + ri*(mTriangles[t].v1 - mTriangles[t].v0);
					vector<double> u, v, w;
					double uu, vv, wu, wv, uv, denominator;

					u = mTriangles[t].v1 - mTriangles[t].v0;
					v = mTriangles[t].v2 - mTriangles[t].v0;
					w = pi - mTriangles[t].v0;

					uu = dot(u,u);
					vv = dot(v,v);
					wu = dot(w,u);
					wv = dot(w,v);
					uv = dot(u,v);
					denominator = uv*uv - uu*vv;
					
					double si = ( uv*wv - vv*wu ) / denominator;
					double ti = ( uv*wu - uu*wv ) / denominator;

					if( si >= 0.0 && si <= 1.0 && ti >= 0.0 && ti <= 1.0 ) {
						render[i][j] = (int) 255.0;
						//cout << "YEP ";
					}
					else {
						//cout << "NOPE " << ti << " " << si << ", ";
					}
				}
			}
		}
	}
	cout << (float) (clock() - renderTime) / CLOCKS_PER_SEC * 1000 << endl;


	// Write a ppm file
	cout << "write" << endl;
	FILE *f = fopen("image.ppm", "w");
	FILE *fc = fopen("imageDebug.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", IMGW, IMGH, 255);
	fprintf(fc, "P3\n%d %d\n%d\n", IMGW, IMGH, 255);
	for (int i = 0; i < IMGH; i++){
		for (int j = 0; j < IMGW; j++){
			fprintf(fc, "%d %d %d ", debugColor[j][i][0], 255, debugColor[j][i][1]);
			fprintf(f, "%d %d %d ", render[j][i], render[j][i], render[j][i]);
		}
	}

	return 0;
}
