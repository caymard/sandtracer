#include <Eigen/Dense>

using namespace Eigen;

class Sphere {
    
private:
    Vector3d mPosition;
    double radius;
    
public:
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