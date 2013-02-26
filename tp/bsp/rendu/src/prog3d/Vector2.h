#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED

#include "Vector3.h"
#include <vector>




/*!
*
* @file
*
* @brief Operations on (x,y) (no distinction between points and vectors)
* @author F. Aubert
*
*/

#include <vector>
#include <iostream>

#define EPSILON_PREC 0.00001

namespace prog3d {
/**
@class Vector2
@brief Operations on (x,y) (no distinction between points and vectors)
*/
class Vector2 {
    double c[2];
    static float fc[2];

    void cnew();
    public:

    /// default constructor
    Vector2();

    /// constructor
    Vector2(double x,double y);
    Vector2(const Vector2 &a,const Vector2 &b);

    /// sets with a-b
    void set(const Vector2 &a,const Vector2 &b);

    /// gets x
    double getX() const;
    double x() const;
    /// gets y
    double getY() const;
    double y() const;
    /// direct access to coordinates
    const double *dv() const;
    /// cast to float *
    const float *fv() const;

    /// sets x
    void setX(double a);
    void x(double x);
    /// sets y
    void setY(double a);
    void y(double y);
    /// copy from copy
    void set(const Vector2 &copy);
    /// sets this
    void set(double x,double y);

    /// adds with a
    void add(const Vector2 &a);
    /// subs with a
    void sub(const Vector2 &a);
    /// this<-a+b
    void add(const Vector2 &a,const Vector2 &b);
    /// this<-a-b
    void sub(const Vector2 &a,const Vector2 &b);
    /// operator copy
    Vector2& operator=(const Vector2 &a);
    /// operator
    void operator+=(const Vector2 &a);
    /// operator
    void operator*=(double k);

    /// Ex : p=a+b
    friend Vector2 operator +(const Vector2 &a,const Vector2 &b);
    /// Ex : p=a-b
    friend Vector2 operator -(const Vector2 &a,const Vector2 &b);
    /// Ex : p=3.0*a
    friend Vector2 operator *(double k,const Vector2 &b);
    /// Ex : p=a*3.0
    friend Vector2 operator *(const Vector2 &b,double k);


    /// get the squared length
    double length2();
    /// give the length
    double length();

    /// get the squared distance between this and a.
    double distance2(const Vector2 &a) const;
    /// distance from this to a
    double distance(const Vector2 &a) const;


    /// normalize
    void normalize();
    /// this set to k*this. Ex : p.scale(3.0)
    void scale(double k);
    /// dot product between this and a. Ex : p1.dot(p2)
    double dot(const Vector2 &a);

    /// set to the mid-point \f$\frac{(n1+n2)}{2}\f$
    void mid(const Vector2& n1,const Vector2 &n2);
    /// set to the mid point this=\f$\frac{this+n1}{2}\f$
    void mid(const Vector2& n1);

    /// print (x,y,z)
    void print();

    /// gives a normal to the edge [this,p2]
    Vector2 normalSegment(const Vector2 &p2);


    /// @brief compatibilité avec cout (affiche les coordonnées).
    friend std::ostream& prog3d::operator <<(std::ostream &s,const Vector2 &t);


    /// destructeur
    virtual ~Vector2();
};

/**
@class AVector2
@brief tableau de pointeurs sur VVector2
*/
class AVector2:public std::vector<Vector2*> {
};



}




#endif // VECTOR2_H_INCLUDED
