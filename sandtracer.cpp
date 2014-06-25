#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

#define IMGW 1440
#define IMGH 900

using namespace std;

struct Sphere {
	// Position
	double x;
	double y;
	double z;

	// Radius
	double radius;
};

struct Camera {
	// Position
	double x;
	double y;
	double z;

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

int main(int argc, char *argv[])
{
	// Sphere definition
	Sphere mSphere;
	mSphere.x = 0;
	mSphere.y = 0;
	mSphere.z = -20;
	mSphere.radius = 2;

	// Camera definition
	Camera mCamera;
	mCamera.x = 0;
	mCamera.y = 0;
	mCamera.z = 0;
	
	mCamera.d = 0.001;
	mCamera.w = 0.017777;
	mCamera.h = 0.01;
	mCamera.pw = IMGW;
	mCamera.ph = IMGH;

	// Rendered image
	cout << "init" << endl;
	vector<vector<int>> render(IMGW, vector<int>(IMGH, 0));
	vector<vector<vector<int>>> debugColor(IMGW, vector<vector<int>>(IMGH, vector<int>(3,0)));
	int counter = 0;

	// Pixel pos
	vector<double> ba(3, 0), u;
	double normu;

	// cam to sphere vector
	ba[0] = mSphere.x - mCamera.x;
	ba[1] = mSphere.y - mCamera.y;
	ba[2] = mSphere.z - mCamera.z;

	// For each pixel try something cool
	cout << "render" << endl;
	#pragma omp parallel for schedule(dynamic,10000) private(u,normu) shared(mSphere,mCamera,debugColor,render)
	for (int i = 0; i < IMGW; i++){
		for (int j = 0; j < IMGH; j++){
			u.resize(3);

			// line direction
			u[0] = mCamera.x - i*(mCamera.w / mCamera.pw) + mCamera.w / 2.0 - mCamera.x;
			u[1] = mCamera.y - j*(mCamera.h / mCamera.ph) + mCamera.h / 2.0 - mCamera.y;
			u[2] = mCamera.z - mCamera.d - mCamera.z;

			// debug color
			normu = norm(u);
			debugColor[i][j][0] = (((u[0]/normu)+1.0)/2.0)*255;
			debugColor[i][j][1] = (((u[1]/normu)+1.0)/2.0)*255;
			debugColor[i][j][2] = (((u[2]/normu)+1.0)/2.0)*255;

			if (norm(cross(ba, u)) / normu < mSphere.radius){
				render[i][j] = (int)255.0 ;//* (1-distToSphere / mSphere.radius);
			}
			else
				render[i][j] = 0;
		}
	}


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