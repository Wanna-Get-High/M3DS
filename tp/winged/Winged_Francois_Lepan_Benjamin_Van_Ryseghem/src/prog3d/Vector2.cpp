#include "Vector2.h"
#include <math.h>
#include "Error.h"

/**


 @author F. Aubert

*/



using namespace prog3d;
using namespace std;


float Vector2::fc[2];

void Vector2::cnew() {
    this->set(0,0);
}

Vector2::Vector2() {
    cnew();
}

Vector2::Vector2(double x,double y) {
    cnew();
    this->set(x,y);
}

Vector2::Vector2(const Vector2 &a,const Vector2 &b) {
  this->set(b);
  this->sub(a);
}

void Vector2::set(const Vector2 &a,const Vector2 &b) {
  this->set(a);
  this->sub(b);
}


void Vector2::set(double x,double y) {
    c[0]=x;c[1]=y;
}

void Vector2::set(const Vector2 &copy) {
    this->set(copy.getX(),copy.getY());
}

double Vector2::getX() const { return c[0];}
double Vector2::getY() const { return c[1];}
const double *Vector2::dv() const {return c;}

const float *Vector2::fv() const {
    fc[0]=c[0];fc[1]=c[1];
    return fc;
}

void Vector2::setX(double a) { c[0]=a;}
void Vector2::setY(double a) { c[1]=a;}

double Vector2::length2() {
    return c[0]*c[0]+c[1]*c[1];
}

double Vector2::length() {
    return sqrt(this->length2());
}

void Vector2::normalize() {
    double d=this->length();
    if (fabs(d)<EPSILON_PREC) {
        throw "Normale nulle";
    }
    c[0]/=d;
    c[1]/=d;

}


void Vector2::add(const Vector2 &a) {
    this->set(this->getX()+a.getX(),
    this->getY()+a.getY());
}

void Vector2::add(const Vector2 &a,const Vector2 &b) {
    this->set(b.getX()+a.getX(),
        b.getY()+a.getY());
}

void Vector2::sub(const Vector2 &a) {
    this->set(this->getX()-a.getX(),
        this->getY()-a.getY());
}


void Vector2::sub(const Vector2 &a,const Vector2 &b) {
    this->set(a.getX()+b.getX(),
        a.getY()+b.getY());
}

double Vector2::dot(const Vector2 &a) {
    return getX()*a.getX()+getY()*a.getY();
}


void Vector2::scale(double k) {
    c[0]*=k;
    c[1]*=k;
}

void Vector2::scale(const Vector2 &k) {
  c[0]*=k.x();
  c[1]*=k.y();
}

void Vector2::invScale(double k) {
  c[0]=k/c[0];
  c[1]=k/c[1];
}



void Vector2::mid(const Vector2& n1,const Vector2 &n2) {
    this->add(n1,n2);
    this->scale(0.5);
}


void Vector2::mid(const Vector2& n1) {
    this->add(n1);
    this->scale(0.5);
}



void Vector2::print() {
    cout << "x=" << this->getX() << ",y=" << this->getY() << endl;
}

Vector2::~Vector2() {}

double Vector2::x() const {
  return c[0];
}

double Vector2::y() const {
  return c[1];
}

void Vector2::x(double x) {
  c[0]=x;
}

void Vector2::y(double y) {
  c[1]=y;
}


//namespace prog3d {
Vector2 prog3d::operator +(const Vector2 &a,const Vector2 &b) {
    Vector2 p(a);
    p.add(b);
    return p;
}

Vector2 prog3d::operator -(const Vector2 &a,const Vector2 &b) {
    Vector2 p(a);
    p.sub(b);
    return p;
}

Vector2 prog3d::operator *(double k,const Vector2 &b) {
    Vector2 p(b);
    p.scale(k);
    return p;
}
//}

Vector2 prog3d::operator *(const Vector2 &b,double k) {
    return k*b;
}


Vector2 prog3d::operator /(const Vector2 &b,double k) {
  Vector2 res(b);
  res.scale(1.0/k);
  return res;
}

Vector2 prog3d::operator *(const Vector2 &a,const Vector2 &b) {
  Vector2 res(a);
  res.scale(b);
  return res;
}

Vector2 prog3d::operator /(const Vector2 &a,const Vector2 &b) {
  Vector2 res;
  res.x(a.x()/b.x());
  res.y(a.y()/b.y());
  return res;
}



Vector2 &Vector2::operator=(const Vector2 &a) {
    this->set(a);
    return *this;
}


ostream& prog3d::operator <<(std::ostream &s,const Vector2 &q) {
    s << "(" << q.getX() << "," << q.getY() << ")";
    return s;
}


// donne un vecteur normé orthogonal au segment [this,P2].
Vector2 Vector2::normalSegment(const Vector2 &p2) {
    Vector2 res;
    res.set(this->getY()-p2.getY(),p2.getX()-this->getX());
    res.normalize();
    return res;
}

double Vector2::distance2(const Vector2 &a) const {
    Vector2 l;
    l.set(*this,a);
    return l.length2();
}

double Vector2::distance(const Vector2 &a) const {
    Vector2 l;
    l.set(*this,a);
    return l.length();
}





