#include <Eigen/Dense>

using namespace Eigen;

class Sphere {
    
public:
    Vector3d mPosition;         // position of the center
    double mRadius;             // radius of the sphere
    
    /**
     * Create a sphere from position and radius
     * @param x         position in x
     * @param y         position in y
     * @param z         position in z
     * @param radius    size of the radius
     */
    Sphere(double x, double y, double z, double radius);
    
    /**
     * Create a sphere from a position vector and radius
     * @param position  3-sized vector for position
     * @param radius    size of the radius
     */
    Sphere(Vector3d position, double radius);
    
    /**
     * Create a default sphere (position = origin, radius = 1.0)
     */
    Sphere();
    
};


class Triangle {
    
private:
    Vector3d mu;        // first vertice
    Vector3d mv;        // second vertice
    Vector3d mw;        // third vertice
    Vector3d mn;        // normal vector
    
public:
    /**
     * Create a triangle from three vertices
     * @param u     first vertice
     * @param v     second vertice
     * @param w     third vertice
     */
    Triangle(Vector3d u, Vector3d v, Vector3d w);
    
    /**
     * Create a triangle from three vertices and specify the normal vector
     * @param u     first vertice
     * @param v     second vertice
     * @param w     third vertice
     * @param n     normal vector
     */
    Triangle(Vector3d u, Vector3d v, Vector3d w, Vector3d n);
    
    /**
     * Create a triangle with dfefault values (xOz)
     */
    Triangle();
    
    /**
     * Compute the normal of the triangle
     */
    void computeNormal();
    
};

class Camera {
    
public:
    Vector3d mPosition;     // position of focal point
    double mD;              // depth : distance between sensor and focal point
    double mW;              // width of sensor
    double mH;              // height of sensor
    double mPW;             // horizontal resolution of sensor
    double mPH;             // vertical resolution of sensor
    
    /**
     * Create a camera with all parameters
     * @param position  position of focal point
     * @param d         distance between sensor and focal point
     * @param w         width of sensor
     * @param h         height of sensor
     * @param pw        horizontal resolution of sensor
     * @param ph        vertical resolution of sensor
     */
    Camera(Vector3d position, double d, double w, double h,
            double pw, double ph);
    
    /**
     * Create a camera with a position and default parameters
     * @param position  position of focal point
     */
    Camera(Vector3d position);
    
    /**
     * Create a camera with default position and parameters
     */
    Camera();
};