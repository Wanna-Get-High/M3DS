#include <stdlib.h>
#include "Quaternion.h"
#include <math.h>
#include "Matrix4.h"
#include "Vector3.h"
#include "Error.h"
#include <math.h>

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

#define MY_PI 3.141592653


Quaternion::Quaternion(double ww,double xx, double yy, double zz) {
  set(ww,xx,yy,zz);
  normalize();
}

void Quaternion::set(double ww,double xx,double yy,double zz) {
  _x=xx;
  _y=yy;
  _z=zz;
  _w=ww;
}

Quaternion::Quaternion(double a,const Vector3 &n) {
  set(a,n.x(),n.y(),n.z());
}

Quaternion::Quaternion(const Quaternion &q) {
    this->set(q);
}



double Quaternion::length() const {
  return sqrt(_w*_w+_x*_x+_y*_y+_z*_z);
}

double Quaternion::lengthXYZ() const {
  return sqrt(_x*_x+_y*_y+_z*_z);
}

void Quaternion::normalize() {
  double n=length();
  _w/=n;
  _x/=n;
  _y/=n;
  _z/=n;
}

Quaternion prog3d::normalize(const Quaternion &q) {
  Quaternion result;
  double l=q.length();
  result._w=q._w/l;
  result._x=q._x/l;
  result._y=q._y/l;
  result._z=q._z/l;
  return result;
}

void Quaternion::set(double scalar,const Vector3 &v) {
    _w=scalar;
    _x=v.x();_y=v.y();_z=v.z();
}

void Quaternion::copyTo(Matrix4 *m) {
    m->set(*this);
}



void Quaternion::copyToAngleAxis(double *angle,Vector3 *u) const {
  *angle=2.0*acos(_w);
  double s=sqrt(1-_w*_w);
  double ax,ay,az;
  if (s<0.0001) {
    ax=1.0;
    ay=0.0;
    az=0.0;
  } else {
    ax=_x/s;
    ay=_y/s;
    az=_z/s;
  }
  u->set(ax,ay,az);

  *angle=*angle*180.0/MY_PI;
}

// from AxisAngle
void Quaternion::setFromAngleAxis(double angle,const Vector3 &v){
  double a_radian=(angle-360.0*int(angle/360.0))*MY_PI/180.0;
  Vector3 v_unit(v);
  v_unit.normalize();
  _w=cos(a_radian/2);
  double ss=sin(a_radian/2);
  _x=v_unit.x()*ss;
  _y=v_unit.y()*ss;
  _z=v_unit.z()*ss;
  this->normalize();
}

Quaternion Quaternion::fromAngleAxis(double angle,const Vector3 &v) {
  Quaternion q;
  q.setFromAngleAxis(angle,v);
  return q;
}

void Quaternion::setRotation(double angle, const Vector3 &u) {
  this->setFromAngleAxis(angle,u);
}

ostream &prog3d::operator <<(ostream &s,const Quaternion &q) {
  s<<"(w= " << q.w() << " x= " << q.x() << " y= " << q.y() << " z= " << q.z() << ")";
  s<<"norme=" << q.length() << endl;
  return s;
}

void Quaternion::set(const Matrix4 &src) {
  double m[9];
  m[0]=src[0];m[1]=src[1];m[2]=src[2];
  m[3]=src[4];m[4]=src[5];m[5]=src[6];
  m[6]=src[8];m[7]=src[9];m[8]=src[10];
  double trace=m[0]+m[4]+m[8]+1;
  _w=sqrt(trace)/2.0;

  double w4=4.0*_w;
  _x=(m[7]-m[5])/w4;
  _y=(m[2]-m[6])/w4;
  _z=(m[3]-m[1])/w4;
  normalize();
}

Quaternion Quaternion::fromMatrix(const Matrix4 &m) {
    Quaternion res;
    res.set(m);
    return res;
}





// mult quaternion
void Quaternion::mul(const Quaternion &q1,const Quaternion &q2)
{
  if ((&q1==this) || (&q2==this)) error("MULT ref=this",__LINE__,__FILE__);
//Q1 * Q2 =( w1.w2 - v1.v2, w1.v2 + w2.v1 + v1*v2)
	_w=  q1.w()*q2.w() - q1.x()*q2.x() - q1.y()*q2.y() - q1.z()*q2.z();
	_x = q1.w()*q2.x() + q1.x()*q2.w() + q1.y()*q2.z() - q1.z()*q2.y();
	_y = q1.w()*q2.y() + q1.y()*q2.w() + q1.z()*q2.x() - q1.x()*q2.z();
	_z = q1.w()*q2.z() + q1.z()*q2.w() + q1.x()*q2.y() - q1.y()*q2.x();
}


void Quaternion::mul(const Quaternion &q) {
  Quaternion q2;
  q2.set(*this);
  this->mul(q2,q);
  this->normalize();
}

void Quaternion::mulLeft(const Quaternion &q) {
  Quaternion q2;
  q2.set(*this);
  this->mul(q,q2);
  this->normalize();
}

Quaternion prog3d::operator *(const Quaternion &q1,const Quaternion &q2) {
    Quaternion res;
    res.mul(q1,q2);
    return res;
}

Quaternion prog3d::operator +(const Quaternion &q1,const Quaternion &q2) {
    Quaternion res;
    res.add(q1,q2);
    return res;
}

// initializer
void Quaternion::set(const Quaternion &q) {
  set(q.w(),q.x(),q.y(),q.z());
}

// conjugate
void Quaternion::conjugate() {
  _x=-_x;
  _y=-_y;
  _z=-_z;
}

Quaternion prog3d::conjugate(const Quaternion &q) {
  Quaternion result;
  result.set(q.w(),-q.x(),-q.y(),-q.z());
  return result;
}


// square norme
double Quaternion::length2() {
  return _w*_w+_x*_x+_y*_y+_z*_z;
}


/// multiply and add (no normalized)
void Quaternion::mad(double alpha,const Quaternion &q) {
  this->mul(alpha);
  this->add(q);
}

// interpolation unit quaternion in [0,1]
void Quaternion::mix(const Quaternion &q1, const Quaternion &q2,double lambda) {
  Quaternion temp(q2);
  this->set(q1);
  temp.mul(lambda);
  this->mad(1.0-lambda,temp);
  this->normalize();
}


// scale
void Quaternion::scale(double s) {
	_w*=s;
	_x*=s;
	_y*=s;
	_z*=s;
}

void Quaternion::mul(double s) {
	_w*=s;
	_x*=s;
	_y*=s;
	_z*=s;
}

void Quaternion::add(const Quaternion &q1,const Quaternion &q2) {
	_w=q1.w()+q2.w();
	_x=q1.x()+q2.x();
	_y=q1.y()+q2.y();
	_z=q1.z()+q2.z();
}

void Quaternion::add(const Quaternion &q) {
	_w+=q.w();
	_x+=q.x();
	_y+=q.y();
	_z+=q.z();
}


Quaternion prog3d::operator *(double k,const Quaternion &q) {
    Quaternion res(q);
    res.scale(k);
    return res;
}

Quaternion prog3d::operator *(const Quaternion &q,double k) {
    Quaternion res(q);
    res.scale(k);
    return res;
}

Vector3 prog3d::operator *(const Quaternion &q,const Vector3 &u) {
  Quaternion uq=Quaternion(0,u);
  Quaternion resq=q*uq*conjugate(q);
  return Vector3(resq.x(),resq.y(),resq.z());
}

void Quaternion::transform(Vector3 *u) {
  *u=(*this)*(*u);
}


void Quaternion::set(const Vector3 &v1,const Vector3 &v2) {

  double angle=v1.angle(v2,v2.cross(v1));
  Vector3 axe=cross(v1,v2);
  if (axe.length()<0.0001) {
	  this->setIdentity();
  }
  else {
	  axe.normalize();
	  this->set(cos(angle/2),sin(angle/2)*axe);
  }
  /*

    Vector3 u;
    Vector3 vv1(v1);
    Vector3 vv2(v2);
    vv1.normalize();
    vv2.normalize();
    vv2=0.5*(vv1+vv2);
    vv2.normalize();
    u.cross(vv1,vv2);
    this->set(vv1.dot(vv2),u);
    this->normalize();
    */
}

void Quaternion::setIdentity() {
  this->set(1,0,0,0);
}

void Quaternion::set(const Vector3 &i,const Vector3 &j,const Vector3 &k) {
  double t=i.x()+j.y()+k.z()+1;
  double s,x,y,z,w;

  if (t>0) {
    s=1.0/(2.0*sqrt(t));

    x=(j.z()-k.y())*s;
    y=(k.x()-i.z())*s;
    z=(i.y()-j.x())*s;
    w=1.0/(4.0*s);
  }
  else {
    if ((i.x()>=j.y()) && (i.x()>=k.z())) {
      s=sqrt(1+i.x()-j.y()-k.z())*2.0;
      x=1.0/(2.0*s);
      y=(j.x()-i.y())/s;
      z=(k.x()-i.z())/s;
      w=(k.y()-j.z())/s;
    } else
    if ((j.y()>=i.x()) && (j.y()>=k.z())) {
      s=sqrt(1-i.x()+j.y()-k.z())*2.0;
      y=1.0/(2.0*s);
      x=(j.x()-i.y())/s;
      w=(k.x()-i.z())/s;
      z=(k.y()-j.z())/s;
    } else {
      s=sqrt(1-i.x()-j.y()+k.z())*2.0;
      z=1.0/(2.0*s);
      w=(j.x()-i.y())/s;
      x=(k.x()-i.z())/s;
      y=(k.y()-j.z())/s;
    }


  }
  set(w,x,y,z);
  normalize();
}


void Quaternion::rotate(double angle,const Vector3 &u) {
  Quaternion q1;
  q1.setFromAngleAxis(angle,u);
  this->mul(q1);
}


Quaternion Quaternion::identity() {
  Quaternion q;
  q.setIdentity();
  return q;
}

Matrix4 Quaternion::toMatrix() {
  return Matrix4::fromQuaternion(*this);
}




