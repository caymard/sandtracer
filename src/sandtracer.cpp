#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

#include "geometry.h"

#define IMGW 1280
#define IMGH 720

using namespace std;

int main(int argc, char *argv[])
{
	int renderTime = clock();
        
	// Sphere definition
	Sphere mSphere(0, 0, -20, 0.25);

	// Camera definition
	Camera mCamera(Vector3d(0,0,0), 0.01, 0.00177777, 0.001, IMGW, IMGH);

	// Rendered image
	cout << "init" << endl;
	vector<vector<int> > render(IMGW, vector<int>(IMGH, 0));
	vector<vector<vector<int> > > debugColor(IMGW, vector<vector<int> >(IMGH, vector<int>(3,0)));

	// Pixel pos
	Vector3d lookat, dir;
	double normdir;

	// cam to sphere vector
	lookat(0) = mSphere.mPosition(0) - mCamera.mPosition(0);
	lookat(1) = mSphere.mPosition(1) - mCamera.mPosition(1);
	lookat(2) = mSphere.mPosition(2) - mCamera.mPosition(2);

	// For each pixel try something cool
	cout << "render parallel" << endl;
	#pragma omp parallel for private(dir,normdir) shared(mSphere, mCamera,debugColor,render)
	for (int i = 0; i < IMGW; i++){
		for (int j = 0; j < IMGH; j++){
			dir.resize(3);

			// line direction
			dir(0) = (double) - i*(mCamera.mW / mCamera.mPW) + mCamera.mW / 2.0;
			dir(1) = (double) - j*(mCamera.mH / mCamera.mPH) + mCamera.mH / 2.0;
			dir(2) = (double) - mCamera.mD;

			// debug color
			normdir = dir.norm();
			debugColor[i][j][0] = (((dir[0]/normdir)+1.0)/2.0)*255;
			debugColor[i][j][1] = (((dir[1]/normdir)+1.0)/2.0)*255;
			debugColor[i][j][2] = (((dir[2]/normdir)+1.0)/2.0)*255;

			if ( (lookat.cross(dir)).norm() / normdir < mSphere.mRadius){
				render[i][j] = (int)255.0 ;//* (1-distToSphere / mSphere.radius);
			}
			else {
				render[i][j] = 0;
			}
			dir = dir / normdir;
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

        // Return wall time (=! real time if openmp is on)
	cout << (float) (clock() - renderTime) / CLOCKS_PER_SEC * 1000 << endl;
	return 0;
}
