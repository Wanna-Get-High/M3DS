#ifndef Matrix4_H_INCLUDED
#define Matrix4_H_INCLUDED

#include <vector>
#include <string>
#include <stack>

/*!
*
* @file
*
* @brief Matrix 4x4
* @author F. Aubert
*
*/

namespace prog3d {

class Vector4;
class Vector3;
class Vector3;
class Quaternion;
class Plane;
class Matrix3;

/**
@class Matrix4
@brief Matrix 4x4
*/
class Matrix4 {
    double m[16]; ///< coefficients (column order)

    static float mf[16]; ///< for casting in float

    std::stack<double *> _stack; /// for push/pop

    void init();

    static Matrix4 _compute; /// to compute predefined translation/scale/rotation matrix (avoid creation)

    public:
    /// default constructor
    Matrix4();
    /// copy constructor
    Matrix4(const Matrix4 &m);

    /// set Matrix with vectors (o,i,j,k) (if frame 1=(o,i,j,k) given in frame 0 then this=1->0)
    void set(const Vector3 &o,const Vector3 &i,const Vector3 &j,const Vector3 &k);

    /// returns the i-th coefficient
    double get(unsigned int i) const;
    inline double operator[](unsigned int i) const {return m[i];}
    /// returns the ith row
    Vector4 getRow(unsigned int i);
    /// direct access to coefficients
    const double *dv() const;
    /// cast to float *
    const float *fv() const;


    /// set copy
    void set(const Matrix4 &a);
    /// copy the 16 values from v to this
    void set(double *v);
    void set(float *v);

    /// set the ith with val
    inline void set(int i,double val) {m[i]=val;}
    /// set from quaternion q
    void set(const Quaternion &q);
    /// transpose this
    void transpose();
    /// invert this
    void invert();
    /// returnns the inverse (this unchanged)
    Matrix4 inverse();
    /// set with the inverse of a
    void invert(const Matrix4 &a);
    /// set the matrix with the rotation matrix from v1 and v2 (axis is \f$ v1\wedge v2\f$, and angle is (v1,v2))
    void setRotation(const Vector3 &v1,const Vector3 &v2);
    /// set the matrix with the rotation from angle/axis (angle in degree)
    void setRotation(float angle,const Vector3 &axis);
    /// set the matrix with the scale matrix (kx,ky,kz)
    void setScale(double kx,double ky,double kz);
    void setScale(const Vector3 &s);
    /// set the matrix with the translation matrix
    void setTranslation(const Vector3 &t);

    /// p is transformed by this (w is the 4th coordinate : 1 for a point, 0 for a direction).
    void transformPoint(Vector3 *p) const;
    void transformDirection(Vector3 *p) const;
    /// p is transformed by this
    void transform(Vector4 *p) const;

    /// returns the transformation of the point p (or the direction if w=0)
    Vector3 transformPoint(const Vector3 &p) const;
    Vector3 transformDirection(const Vector3 &p) const;
    /// returns the transformation of the point p
    Vector4 transform(const Vector4 &p) const;

    /// swap columns i and j
    void swapColumn(int i,int j);
    /// multiply column i by k
    void scaleColumn(int i,double k);
    /// multiply row i by k
    void scaleRow(int i,double k);
    /// Scale then sub the columns i and j : Ci<-Ci-k*Cj
    void subScaleColumn(int i,int j,double k);
    /// Scale then sub the lines i and j : Li<-Li-k*Lj
    void subScaleLine(int i,int j,double s);
    /// set the ith row
    void setRow(int i,double x,double y,double z,double w);
    /// set the ith column
    void setColumn(int i,double x,double y,double z,double w);


    /// set to identity
    void setIdentity();
    /// right mul : this<-this*a
    void mul(const Matrix4 &a);
    /// left mul : this<-a*this
    void mulLeft(const Matrix4 &a);
    /// set this<-A*B
    void mul(const Matrix4 &a,const Matrix4 &b);

    /// printf
    void print(std::string mesg) const;


    /// compute a translation matrix
    static const Matrix4 &translation(const Vector3 &v);
    static const Matrix4 &scaling(const Vector3 &v);
    static const Matrix4 &rotation(double angle,const Vector3 &axe);
    static const Matrix4 &rotation(const Quaternion &q);

    static Matrix4 fromQuaternion(const Quaternion &q);

    /// cumulate with a translation
    void translate(const Vector3 &u);

    /// cumulate with rotation (in degree)
    void rotate(double angle,const Vector3 &u);

    /// cumulate with scale
    void scale(const Vector3 &u);

    /// cumulate with the lookAt matrix :
    void lookAt(const Vector3 &position,const Vector3 &at,const Vector3 &up);

    /// mix : this <- (1-t)*m1+t*m2
    void mix(const Matrix4 &m1,const Matrix4 &m2,double t);

    /// set to the frustum matrix (prepared for 1/z interpolation, i.e. same as openGL 2.1)
    void frustum(double left,double right,double bottom,double top,double near,double far);
    /// set to the orthogonal projection matrix
    void ortho(double left,double right,double bottom,double top,double near,double far);

    // returns the normal matrix. i.e. (the 3x3 matrix from transpose(inverse(this))
    Matrix3 normalMatrix() const;

    void projectionPoint(const Plane &plane,const Vector3 &point);
    void projectionDirection(const Plane &p,const Vector3 &u);


    /// saves the content of this on the top of a stack
    void push();
    /// restore the content of this from the top of the stack
    void pop();


    /// destructor
    virtual ~Matrix4();

    /// operator * : returns m1*m2
    friend Matrix4 operator *(const Matrix4 &m1,const Matrix4 &m2);
};



}

#endif // Matrix4_H_INCLUDED

