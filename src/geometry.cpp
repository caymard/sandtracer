#include "geometry.h"

Sphere::Sphere(double x, double y, double z, double radius) {
    mPosition = Vector3d(x, y, z);
    mRadius = radius;
}

Sphere::Sphere(Vector3d position, double radius) {
    mPosition = Vector3d(position);
    mRadius = radius;
}

Sphere::Sphere() {
    mPosition = Vector3d(0,0,0);
    mRadius = 1;
}

Camera::Camera(Vector3d position, double d, double w, double h,
            double pw, double ph) {
    mPosition = Vector3d(position);
    mD = d;
    mW = w;
    mH = h;
    mPW = pw;
    mPH = ph;    
}

Camera::Camera(Vector3d position) {
    mPosition = Vector3d(position);
    mD = 0.01;
    mW = 0.0017777;
    mH = 0.001;
    mPW = 640;
    mPH = 360;
}

Camera::Camera() {
    mPosition = Vector3d(0, 0, 0);
    mD = 0.01;
    mW = 0.0017777;
    mH = 0.001;
    mPW = 640;
    mPH = 480;
}