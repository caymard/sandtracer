#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

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
	mSphere.z = -40;
	mSphere.radius = 2;

	// Camera definition
	Camera mCamera;
	mCamera.x = 0;
	mCamera.y = 0;
	mCamera.z = 0;
	
	mCamera.d = 0.03;
	mCamera.w = 0.017777;
	mCamera.h = 0.01;
	mCamera.pw = IMGW;
	mCamera.ph = IMGH;

	// Rendered image
	int render[IMGW][IMGH];
	int counter = 0;

	// Pixel pos
	double px, py, pz;

	// For each pixel try something cool
	for (int i = 0; i < IMGW; i++){
		for (int j = 0; j < IMGH; j++){
			bool touched = false;
			vector<double> ba(3, 0), u(3, 0);
			double distToSphere;

			// cam to sphere vector
			ba[0] = mSphere.x - mCamera.x;
			ba[1] = mSphere.y - mCamera.y;
			ba[2] = mSphere.z - mCamera.z;

			// Find pixel position in space
			pz = mCamera.z - mCamera.d;
			px = mCamera.x - i*(mCamera.w / mCamera.pw) + mCamera.w / 2.0;
			py = mCamera.y - j*(mCamera.h / mCamera.ph) + mCamera.h / 2.0;

			// line direction
			u[0] = px - mCamera.x;
			u[1] = py - mCamera.y;
			u[2] = pz - mCamera.z;

			distToSphere = norm(cross(ba, u)) / norm(u);

			touched = (distToSphere < mSphere.radius);
			if (touched){
				render[i][j] = (int)255.0 * distToSphere / mSphere.radius;
			}
			else
				render[i][j] = 0;
		}
	}


	// Write a ppm file
	FILE *f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", IMGW, IMGH, 255);
	for (int i = 0; i < IMGW; i++){
		for (int j = 0; j < IMGH; j++){
			fprintf(f, "%d %d %d ", render[i][j], render[i][j], render[i][j]);
		}
	}

	return 0;
}