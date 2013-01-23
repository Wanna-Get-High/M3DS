#include "Vector3.h"
#include "Error.h"
#include <math.h>
#include <iostream>

using namespace prog3d;
using namespace std;

float Vector3::fc[3];




void Vector3::cnew() {
//    c.resize(3);
    this->set(0,0,0);
}

Vector3::Vector3() {
    cnew();
}


Vector3::Vector3(double x,double y,double z) {
    cnew();
    this->set(x,y,z);
}

Vector3::Vector3(const Vector3 &t) {
    cnew();
    this->set(t);
}

Vector3::Vector3(const Vector2 &t) {
    cnew();
    this->set(t.x(),t.y(),0.0);
}


Vector3::Vector3(const Vector3 &a,const Vector3 &b) {
    this->setVector(a,b);
}

void Vector3::setVector(const Vector3 &a,const Vector3 &b) {
    this->set(b);
    this->sub(a);
}


void swap(int &a,int &b) {
  int c;
  c=a;
  a=b;
  b=c;
}


void Vector3::set(const Vector3 &a,const Vector3 &b) {
  this->set(b);
  this->sub(a);
}


void Vector3::set(double x,double y,double z) {
    c[0]=x;c[1]=y;c[2]=z;
}

void Vector3::set(const Vector3 &copy) {
    this->set(copy.x(),copy.y(),copy.z());
}

void Vector3::set(int i,double coordinate) {
    c[i]=coordinate;
}


float *Vector3::fv() const {
    fc[0]=c[0];fc[1]=c[1];fc[2]=c[2];
    return fc;
}


double Vector3::length2() const {
    return c[0]*c[0]+c[1]*c[1]+c[2]*c[2];
}

double Vector3::length() const {
    return sqrt(this->length2());
}

double Vector3::distance2(const Vector3 &a) const {
  return Vector3(*this,a).length2();
}

double Vector3::distance(const Vector3 &a) const {
  return Vector3(*this,a).length();
}


const Vector3 &Vector3::normalize() {
    double d=this->length();
   if (fabs(d)<EPSILON_PREC) {
        throw Error("Norme nulle",__LINE__,__FILE__);
    }
    c[0]/=d;
    c[1]/=d;
    c[2]/=d;
    return *this;

}

void Vector3::add(double x,double y,double z) {
    c[0]+=x;c[1]+=y;c[2]+=z;
}

void Vector3::add(const Vector3 &a) {
	c[0]+=a.x();
	c[1]+=a.y();
	c[2]+=a.z();
}

void Vector3::add(const Vector3 &a,const Vector3 &b) {
    this->set(b.x()+a.x(),
        b.y()+a.y(),
        b.z()+a.z());
}

void Vector3::sub(const Vector3 &a) {
    this->set(this->x()-a.x(),
        this->y()-a.y(),
        this->z()-a.z());
}


void Vector3::sub(const Vector3 &a,const Vector3 &b) {
    this->set(a.x()+b.x(),
        a.y()+b.y(),
        a.z()+b.z());
}



void Vector3::scale(double k) {
    c[0]*=k;
    c[1]*=k;
    c[2]*=k;
}

void Vector3::mul(double k) {
    c[0]*=k;
    c[1]*=k;
    c[2]*=k;
}


void Vector3::scale(double kx,double ky,double kz) {
    c[0]*=kx;
    c[1]*=ky;
    c[2]*=kz;
}



void Vector3::scaleAdd(double k,const Vector3 &a) {
    this->scale(k);
    this->add(a);
}



// rotation autour de Y (classiquement rotation de profil)
Vector3 Vector3::rotationY(float angle) const {
    Vector3 res;
    res.x(cos(angle)*x()-sin(angle)*z());
    res.y(y());
    res.z(sin(angle)*x()+cos(angle)*z());
    return res;
}

void Vector3::mid(const Vector3& n1,const Vector3 &n2) {
    this->add(n1,n2);
    this->scale(0.5);
}


void Vector3::mid(const Vector3& n1) {
    this->add(n1);
    this->scale(0.5);
}


double Vector3::dot(const Vector3 &a) const {
    return x()*a.x()+y()*a.y()+z()*a.z();
}

void Vector3::cross(const Vector3 &a,const Vector3 &b) {
    this->set(a.y()*b.z()-a.z()*b.y(),
        a.z()*b.x()-b.z()*a.x(),
        a.x()*b.y()-b.x()*a.y());
}

Vector3 Vector3::cross(const Vector3 &u) const {
  Vector3 res;
  res.cross(*this,u);
  return res;
}


void Vector3::mul(const Vector3 &u) {
  c[0]*=u.x();
  c[1]*=u.y();
  c[2]*=u.z();
}


void Vector3::print(string mesg) const {
    cout << mesg << "(" << this->x() << "," << this->y() << "," << this->z() << ")" << endl;
}

namespace prog3d {
Vector3 operator +(const Vector3 &a,const Vector3 &b) {
    Vector3 p(a);
    p.add(b);
    return p;
}

Vector3 operator -(const Vector3 &a,const Vector3 &b) {
    Vector3 p(a);
    p.sub(b);
    return p;
}

Vector3 operator *(double k,const Vector3 &b) {
    Vector3 p(b);
    p.scale(k);
    return p;
}

Vector3 operator *(const Vector3 &b,double k) {
    return k*b;
}

Vector3 operator /(const Vector3 &b,double k) {
    return 1.0/k*b;
}


Vector3 &Vector3::operator =(const Vector3 &a) {
    this->set(a);
    return *this;
}

Vector3 &Vector3::operator +=(const Vector3 &a) {
    this->add(a);
    return *this;
}

Vector3 &Vector3::operator *=(double k) {
  this->mul(k);
  return *this;
}

Vector3 &Vector3::operator /=(double k) {
  this->mul(1.0/k);
  return *this;
}

Vector3 operator *(const Vector3 &u,const Vector3 &v) {
  return Vector3(u.x()*v.x(),u.y()*v.y(),u.z()*v.z());
}


Vector3 operator -(const Vector3 &a) {
    Vector3 res(a);
    res.set(-res.x(),-res.y(),-res.z());
    return res;
}


ostream& operator <<(std::ostream &s,const Vector3 &q) {
    s << "(" << q.x() << "," << q.y() << "," << q.z() << ")";
    return s;
}

Vector3 normalize(const Vector3 &t) {
  Vector3 res;
  res.set(t);
  res.normalize();
  return res;
}

}

void Vector3::mad(double k,const Vector3 &t) {
  this->mul(k);
  this->add(t);
}

void Vector3::mix(const Vector3 &t1,const Vector3 &t2,double t) {
  Vector3 temp(t2);
  this->set(t1);
  temp.mul(t);
  this->mad(1.0-t,temp);
}

Vector3::~Vector3() {}


double Vector3::angle(const Vector3 &u,const Vector3 &vertical) const {
  Vector3 u1(u);
  Vector3 v1(*this);

  // calcul cosine par produit scalaire
  u1.normalize();
  v1.normalize();

  // acos donne entre 0 et PI
  double angle=acos(u1.dot(v1));

  // détermination du signe par produit vectoriel par rapport à la "verticale" (i.e. déterminant en 2D avec une verticale=(0,0,1) ))
  Vector3 n=u1.cross(v1);
  if (n.dot(vertical)<0) angle=-angle;

  return angle;
}

void Vector3::point(const Vector4 &u) {
  this->set(u.x()/u.w(),u.y()/u.w(),u.z()/u.w());
}

void Vector3::direction(const Vector4 &u) {
  this->set(u.x(),u.y(),u.z());
}

void Vector3::setMinCoordinate(const Vector3 &a) {
  if (a.x()<this->x()) this->x(a.x());
  if (a.y()<this->y()) this->y(a.y());
  if (a.z()<this->z()) this->z(a.z());
}

void Vector3::setMaxCoordinate(const Vector3 &a) {
  if (a.x()>this->x()) this->x(a.x());
  if (a.y()>this->y()) this->y(a.y());
  if (a.z()>this->z()) this->z(a.z());
}

double Vector3::min(unsigned int *which) {
	unsigned int w;
  if (*c<*(c+1)) {
	  w=(*c<*(c+2))?0:2;
  }
  else {
    w=(*(c+1)<*(c+2))?1:2;
  }
  if (which!=NULL) *which=w;
  return *(c+w);
}

double Vector3::max(unsigned int *which) {
	unsigned int w;
  if (*c>*(c+1)) {
	  w=(*c>*(c+2))?0:2;
  }
  else {
    w=(*(c+1)>*(c+2))?1:2;
  }
  if (which!=NULL) *which=w;
  return *(c+w);
}




