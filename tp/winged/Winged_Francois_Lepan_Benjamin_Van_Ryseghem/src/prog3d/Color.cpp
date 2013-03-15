#include "Color.h"

using namespace prog3d;
using namespace std;

Color::~Color() {
}

Color::Color() {
  _c[0]=0.0;_c[1]=0.0;_c[2]=0.0;_c[3]=0.0;
}

Color::Color(const Vector3 &t,double a) {_c[0]=t.x();_c[1]=t.y();_c[2]=t.z();_c[3]=a;}

Color::Color(double r,double g,double b,double a) { _c[0]=r;_c[1]=g;_c[2]=b;_c[3]=a;}



unsigned char Color::getByteR() const {
  double k=this->r();
  if (k>1) return 255; else if (k<0) return 0;
  return (unsigned char)(k*255);
}

unsigned char Color::getByteG() const {
  double k=this->g();
  if (k>1) return 255; else if (k<0) return 0;
  return (unsigned char)(k*255);
}

unsigned char Color::getByteB() const {
  double k=this->b();
  if (k>1) return 255; else if (k<0) return 0;
  return (unsigned char)(k*255);
}

void Color::setBytev(unsigned char *c) {
    set((double)c[0]/255.0,(double)c[1]/255.0,(double)c[2]/255.0);
}


Color prog3d::operator*(const Color &c1,const Color &c2) {
  return Color(c1.r()*c2.r(),c1.g()*c2.g(),c1.b()*c2.b(),c1.a()*c2.a());
}

Color prog3d::operator *(double k,const Color &c) {
  Color res;
  res.set(k*c.r(),k*c.g(),k*c.b(),k*c.a());
  return res;
}

Color prog3d::operator *(const Color &c,double k) {
  Color res;
  res.set(k*c.r(),k*c.g(),k*c.b(),k*c.a());
  return res;
}

Color prog3d::operator -(const Color &c1,const Color &c2) {
  Color res;
  res.set(c1.r()-c2.r(),c1.g()-c2.g(),c1.b()-c2.b(),c1.a()-c2.a());
  return res;
}

Color prog3d::operator +(const Color &c1,const Color &c2) {
  Color res;
  res.set(c1.r()+c2.r(),c1.g()+c2.g(),c1.b()+c2.b(),c1.a()+c2.a());
  return res;
}

const Color &Color::operator+=(const Color &c) {
  _c[0]+=c.r();
  _c[1]+=c.g();
  _c[2]+=c.b();
  _c[3]+=c.a();
  return *this;
}


const Color &Color::add(const Color &c) {
  r(_c[0]+c.r());
  g(_c[1]+c.g());
  b(_c[2]+c.b());
  return *this;
}

const Color &Color::addSature(const Color &c) {
    this->add(c);
    if (this->r()>1) this->r(1);
    if (this->g()>1) this->g(1);
    if (this->b()>1) this->b(1);
    if (this->r()<0) this->r(0);
    if (this->b()<0) this->g(0);
    if (this->g()<0) this->b(0);
    return *this;
}

ostream& prog3d::operator <<(std::ostream &s,const Color &c) {
    s << "(" << c.r() << "," << c.g() << "," << c.b() << "," << c.a() << ")";
    return s;
}


