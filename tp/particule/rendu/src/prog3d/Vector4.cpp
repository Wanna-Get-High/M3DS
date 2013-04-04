#include "Vector4.h"
#include "Vector3.h"
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

float Vector4::fc[4];

void Vector4::init() {
}

Vector4::Vector4() {
    init();
}

Vector4::Vector4(const Vector3 &p,double w) {
    init();
    this->set(p,w);
}

Vector4::Vector4(const Vector4 &p) {
    init();
    this->set(p);
}

Vector4::Vector4(double x,double y,double z,double w) {
    init();
    c[0]=x;c[1]=y;c[2]=z;c[3]=w;
}

Vector4::~Vector4() {}

void Vector4::set(const Vector3 &p,double w) {
    c[0]=p.x();
    c[1]=p.y();
    c[2]=p.z();
    c[3]=w;
}



void Vector4::set(const Vector4 &p) {
    c[0]=p.getX();
    c[1]=p.getY();
    c[2]=p.getZ();
    c[3]=p.getW();
}

double Vector4::getX() const { return c[0];}
double Vector4::getY() const { return c[1];}
double Vector4::getZ() const { return c[2];}
double Vector4::getW() const { return c[3];}

void Vector4::setX(double x) { c[0]=x;}
void Vector4::setY(double y) { c[1]=y;}
void Vector4::setZ(double z) { c[2]=z;}
void Vector4::setW(double w) { c[3]=w;}

const double *Vector4::dv() const {
    return c;
}

const float *Vector4::fv() const {
    fc[0]=c[0];fc[1]=c[1];fc[2]=c[2];fc[3]=c[3];
    return fc;
}

void Vector4::print(string mesg) const {
    cout << mesg << "(" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << ")\n";
}

Vector3 Vector4::project() {
  Vector3 res;
  res.set(this->getX(),this->getY(),this->getZ());
  res.scale(this->getW());
  return res;
}

ostream& prog3d::operator <<(ostream &s,const Vector4 &p) {
    s << "(" << p.getX() << "," << p.getY() << "," << p.getZ() << "," << p.getW() << ")";
    return s;
}




