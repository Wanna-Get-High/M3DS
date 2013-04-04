#ifndef Plane_H_INCLUDED
#define Plane_H_INCLUDED

#include "Vector3.h"
#include "Line.h"
/*!
*
* @file
*
* @brief opérations sur les Planes am.n=0
* @author F. Aubert
*
*/

namespace prog3d {
  class Line;
  enum ESide {SIDE_PLUS,SIDE_MINUS,SIDE_NONE}; // NONE => not computed

/**
@class Plane
@brief Representation of a plane (based on AM.n=0 : A is a point of the plane, and n its normal)
*/
class Plane {
    Vector3 _n; ///< normal
    Vector3 _a;  ///< point of the plane (named origin).

    public:
    /// destructor
    virtual ~Plane();
    /// default constructor
    Plane();
    /// constructor from a point and its normal Plane(a,n)
    Plane(const Vector3 &a,const Vector3 &n);

    /// get the normal
    const Vector3 &normal() const;
    /// get the origin
    const Vector3 &point() const;
    /// get the normal
    const Vector3 &getN() const;
    /// get the origin
    const Vector3 &getA() const;

    void normal(const Vector3 &norm);
    void point(const Vector3 &p);

    /// set
    void set(const Vector3 &a,const Vector3 &n);
    /// set from 3 points
    void set(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3);
    /// orthogonal projection of the point P
    Vector3 project(const Vector3 &p);
    /// projection of p along the direction u
    Vector3 project(const Vector3 &p,const Vector3 &u);
    /// intersection of the plane with the line d : returns k such P=a+ku
    double interK(const Line &d) const;

    /// returns the intersection point P between the plane and the line d
    Vector3 intersect(const Line &d) const;

    /// signed distance (>0 if p is at the normal side).
    double distance(const Vector3 &p) const;

    /// sign of the point
    ESide side(const Vector3 &p) const;


    friend std::ostream& operator<<(std::ostream &s,const Plane &p);

};

}


#endif // Plane_H_INCLUDED

