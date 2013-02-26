#ifndef VECTOR4_H_INCLUDED
#define VECTOR4_H_INCLUDED

namespace prog3d {
class Vector3;
}

#include <string>
#include "Vector3.h"

/**
@file
@author F. Aubert
@brief Operations on homegeneous co-ordinates (x,y,z,w)
*/

namespace prog3d {

/**
@class Vector4
@brief Operations on homegeneous co-ordinates (x,y,z,w)
*/
class Vector4 {
    double c[4];
    static float fc[4]; // for casting

    void init();
    public:
    /// destructor
    virtual ~Vector4();
    /// default constructor
    Vector4();
    /// construction from Vector3
    Vector4(const Vector3 &p,double w=1.0);

    /// copy constructor
    Vector4(const Vector4 &p);
    /// constructor from (x,y,z,w)
    Vector4(double x,double y,double z=0.0,double w=1.0);

    /// set
    void set(const Vector3 &p,double w=1.0);

    /// copy from
    void set(const Vector4 &p);

    /// set x
    void setX(double x);
    /// set y
    void setY(double y);
    /// set z
    void setZ(double z);
    /// set w
    void setW(double w);

    /// get x
    double getX() const;
    /// get y
    double getY() const;
    /// get z
    double getZ() const;
    /// get w
    double getW() const;

    /// getters
    inline const double &x() const {return c[0];}
    inline const double &y() const {return c[1];}
    inline const double &z() const {return c[2];}
    inline const double &w() const {return c[3];}

    /// setters
    void x(double x) {c[0]=x;}
    void y(double y) {c[1]=y;}
    void z(double z) {c[2]=z;}
    void w(double w) {c[3]=w;}



    /// divide by w (projection of homogeneous co-ordinate)
    Vector3 project();

    /// direct access (Ex : for glVertex4dv)
    const double *dv() const;
    /// cast access (Ex : for glVertex4fv)
    const float *fv() const;

    /// print
    void print(std::string mesg) const;
    /// cout
    friend std::ostream& operator<<(std::ostream &s,const Vector4 &p);

};

}

#endif // POINT4D_H_INCLUDED
