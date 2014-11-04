#include <Eigen/Dense>

using namespace Eigen;

class Sphere {
    
public:
    Vector3d mPosition;
    double mRadius;
    

    Sphere(double x, double y, double z, double radius);
    Sphere(Vector3d position, double radius);
    Sphere();
    
};


class Triangle {
    
private:
    Vector3d mu, mv, mw, mn;
    
public:
    Triangle(Vector3d u, Vector3d v, Vector3d w);
    Triangle(Vector3d u, Vector3d v, Vector3d w, Vector3d n);
    Triangle();
    
    void computeNormal();
    
};

class Camera {
    
public:
    Vector3d mPosition;
    double mD;
    double mW;
    double mH;
    double mPW;
    double mPH;
    

    Camera(Vector3d position, double d, double w, double h,
            double pw, double ph);
    Camera(Vector3d position);
    Camera();
};