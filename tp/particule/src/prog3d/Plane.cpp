#include "Plane.h"
#include <iostream>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/
using namespace std;
using namespace prog3d;



Plane::~Plane() {
}

Plane::Plane() {
}

Plane::Plane(const Vector3 &a,const Vector3 &n) {
    this->set(a,n);
}

void Plane::set(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3) {
    Vector3 u1(p1,p2);
    Vector3 u2(p2,p3);
    _n.cross(u1,u2);
    _a.set(p1);
}

void Plane::set(const Vector3 &a,const Vector3 &n) {
    this->_n.set(n);
    this->_a.set(a);
}

double Plane::interK(const Line &d) const {
    return (_n.dot(_a-d.getA()))/_n.dot(d.getU());
}

double Plane::distance(const Vector3 &p) const {
  return (p-_a).dot(_n)/_n.dot(_n);
}

ESide Plane::side(const Vector3 &p) const {
  Vector3 ap(_a,p);
  if (ap.dot(_n)>=0) return SIDE_PLUS;
  else return SIDE_MINUS;
}

Vector3 Plane::project(const Vector3 &p) {
    Line d(p,_n);
    double k=this->interK(d);
    return k*_n+p;
}

Vector3 Plane::project(const Vector3 &p,const Vector3 &u) {
    Line d(p,u);
    double k=this->interK(d);
//    UtilGL::add(k*u+p);
    return k*u+p;
}

Vector3 Plane::intersect(const Line &d) const {
  double k=this->interK(d);
  return k*d.getU()+d.getA();
}

const Vector3 &Plane::getA() const {
    return _a;
}

const Vector3 &Plane::getN() const {
    return _n;
}

const Vector3 &Plane::normal() const {
  return _n;
}

const Vector3 &Plane::point() const {
  return _a;
}

void Plane::point(const Vector3 &p) {
  _a=p;
}

void Plane::normal(const Vector3 &norm) {
  _n=norm;
}

ostream& prog3d::operator<<(ostream &s,const Plane &p) {
    s << "[POINT=" << p.point() << ",N=" << p.normal() << "]";
    return s;
}





