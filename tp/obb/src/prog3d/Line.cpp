#include "Line.h"
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


Line::~Line() {
}

Line::Line() {
  _a.set(0,0,0);
  _u.set(1,0,0);
}

Line::Line(const Line &l) {
  point(l.point());
  direction(l.direction());
}


Line::Line(const Vector3 &aa,const Vector3 &uu) {
  _u.set(uu);
  _a.set(aa);
}

void Line::set(const Vector3 &aa,const Vector3 &uu) {
  _u.set(uu);
  _a.set(aa);
}





Vector3 Line::point(double k) const {
  return getA()+k*getU();
}



double Line::distance2(const Vector3 &m) const {
  Vector3 am(m);
  am.sub(_a);

  double k=am.dot(_u)/_u.dot(_u);

  Vector3 h(_u);
  h.scaleAdd(k,_a);

  am.set(h);
  am.sub(m);
  return am.length2();

}


void Line::print(string mesg) const {
  cout << mesg << "A=" << this->getA() << ", U=" << this->getU() << endl;
}

ostream& prog3d::operator<<(ostream &s,const Line &l) {
  s << "[A=" << l.getA() << ",U=" << l.getU() << "]";
  return s;
}

bool Line::set(const Plane &p1,const Plane &p2) {
  _u.setCross(p1.normal(),p2.normal());
  if (_u.length2()<0.0001) {
    return false;
  }
  else {
    Vector3 v;
    _u.normalize();
    v.setCross(_u,p1.normal());
    _a.set(p2.intersect(Line(p1.point(),v)));
    return true;
  }
}



