#include "Matrix4.h"
#include <iostream>
#include <cmath>
#include <string>
#include "Vector4.h"
#include "Vector3.h"
#include "Error.h"
#include "Quaternion.h"
#include "Matrix3.h"

#ifdef __linux
    #include <GL/glu.h>
#endif

#if defined(__APPLE__)
    #include <GLUT/glut.h>
#else
    #include <gl/glut.h>
#endif


/*


 F. Aubert

*/
using namespace prog3d;
using namespace std;

#define MY_PI 3.141592653

Matrix4 Matrix4::_compute;

float Matrix4::mf[16];

void Matrix4::init() {
//    m.resize(16);
}

Matrix4::Matrix4() {
    init();
}

Matrix4::~Matrix4() {
}

Matrix4::Matrix4(const Matrix4 &m) {
    init();
    this->set(m);
}

void Matrix4::set(const Vector3 &o,const Vector3 &i,const Vector3 &j,const Vector3 &k) {
  m[0]=i.x();
  m[1]=i.y();
  m[2]=i.z();
  m[3]=0.0;

  m[4]=j.x();
  m[5]=j.y();
  m[6]=j.z();
  m[7]=0;

  m[8]=k.x();
  m[9]=k.y();
  m[10]=k.z();
  m[11]=0.0;

  m[12]=o.x();
  m[13]=o.y();
  m[14]=o.z();
  m[15]=1.0;
}


void Matrix4::set(const Matrix4 &a) {
    for(int i=0;i<16;i++) {
        m[i]=a[i];
    }
}

double Matrix4::get(unsigned int i) const {
    return m[i];
}

const double *Matrix4::dv() const {
    return m;
}



void Matrix4::transformPoint(Vector3 *p) const {
  Vector4 temp;
  temp.set(*p,1);
  transform(&temp);
  p->point(temp);
}

void Matrix4::transformDirection(Vector3 *p) const {
  Vector4 temp;
  temp.set(*p,0);
  transform(&temp);
  p->direction(temp);
}

Vector3 Matrix4::transformPoint(const Vector3 &p) const {
    Vector3 res=p;
    transformPoint(&res);
    return res;
}

Vector3 Matrix4::transformDirection(const Vector3 &p) const {
    Vector3 res=p;
    transformDirection(&res);
    return res;
}



void Matrix4::transform(Vector4 *p) const {
    Vector4 temp;
    temp.set(*p);
    p->x(m[0]*temp.x()+m[4]*temp.y()+m[8]*temp.z()+m[12]*temp.w());
    p->y(m[1]*temp.x()+m[5]*temp.y()+m[9]*temp.z()+m[13]*temp.w());
    p->z(m[2]*temp.x()+m[6]*temp.y()+m[10]*temp.z()+m[14]*temp.w());
    p->w(m[3]*temp.x()+m[7]*temp.y()+m[11]*temp.z()+m[15]*temp.w());
}


Vector4 Matrix4::transform(const Vector4 &p) const {
    Vector4 res=p;
    transform(&res);
    return res;
}

void Matrix4::setIdentity() {
    for(unsigned int a=0;a<16;a++) {
        m[a]=0.0;
    }

    for(int i=0;i<16;i+=5) {
      m[i]=1.0;
    }

}

void Matrix4::set(double *v) {
    for(int i=0;i<16;i++) {
        m[i]=v[i];
    }
}

void Matrix4::set(float *v) {
    for(int i=0;i<16;i++) {
        m[i]=v[i];
    }
}

void Matrix4::print(string mesg) const {
    cout << "matrice " << mesg << endl;
    int index=0;
    for(int i=0;i<4;i++) {
        cout << "(";
        for(int j=0;j<4;j++) {
          cout << m[index] << " ";
          index+=4;
        }
        cout << ")" << endl;
        index-=15;
    }
}

void Matrix4::swapColumn(int i,int j) {
    double swap;
    int c1=i*4;
    int c2=j*4;
    for(int k=0;k<4;k++) {
        swap=m[c1];
        m[c1]=m[c2];
        m[c2]=swap;
        c1++;c2++;
    }
}


void Matrix4::scaleColumn(int i,double k) {
    int c=i*4;
    for(int j=0;j<4;j++) {
        m[c]*=k;
        c++;
    }
}

void Matrix4::scaleRow(int i,double k) {
    int c=i;
    for(int j=0;j<4;j++) {
        m[c]*=k;
        c+=4;
    }
}


void Matrix4::mul(const Matrix4 &a) {
    Matrix4 temp;
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            int tt=i+j*4;
            temp.set(tt,0);
            for(int k=0;k<4;k++) {
                temp.set(tt, temp[tt]+m[i+k*4]*a.get(k+j*4));
            }
        }
    }
    this->set(temp);
}

void Matrix4::mulLeft(const Matrix4 &a) {
    Matrix4 temp;
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            int tt=i+j*4;
            temp.set(tt,0);
            for(int k=0;k<4;k++) {
                temp.set(tt, temp[tt]+a.get(i+k*4)*m[k+j*4]);
            }
        }
    }
    this->set(temp);
}



void Matrix4::mul(const Matrix4 &a,const Matrix4 &b) {
    if ((this==&a) || (this==&b)) error("auto mul : use mul(const Matrix4 &)",__LINE__,__FILE__);
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            int tt=i+j*4;
            this->set(tt,0);
            for(int k=0;k<4;k++) {
                this->set(tt, (*this)[tt]+a[i+k*4]*b.get(k+j*4));
            }
        }
    }
}


void Matrix4::transpose() {
    Matrix4 temp(*this);
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            m[i*4+j]=temp[j*4+i];
        }
    }
}

void Matrix4::invert() {
    Matrix4 temp(*this);
    this->invert(temp);
}

Matrix4 Matrix4::inverse() {
  Matrix4 res;
  res.set(*this);
  res.invert();
  return res;
}


void Matrix4::subScaleColumn(int i,int j,double s) {
    int c1=i*4;int c2=j*4;
    for(int k=0;k<4;k++) {
        m[c1]-=(m[c2]*s);
        c1++;c2++;
    }
}

void Matrix4::subScaleLine(int i,int j,double s) {
    int l1=i;int l2=j;
    for(int k=0;k<4;k++) {
        m[l1]-=(m[l2]*s);
        l1+=4;l2+=4;
    }
}

void Matrix4::setRow(int i,double x,double y,double z,double w) {
    m[i]=x;m[i+4]=y;m[i+8]=z;m[i+12]=w;
}

void Matrix4::setColumn(int i,double x,double y,double z,double w) {
    int i4=i*4;
    m[i4]=x;m[i4+1]=y;m[i4+2]=z;m[i4+3]=w;
}

void Matrix4::invert(const Matrix4 &a) {


  Matrix4 temp;
  temp.set(a);
  this->setIdentity();

  double t,t2;
  int i,j,k;
  int swap;

  // descente
  for(i=0;i<4;i++) {
    double max=temp[i*4+i];
    swap=i;
    for(j=(i+1);j<4;j++) {
        if (fabs(temp[j*4+i])>fabs(max)) {
            max=temp[j*4+i];
            swap=j;
        }
    }
    if (fabs(max)<1e-08)
    	throw Error("matrice inverse",__LINE__,__FILE__);
    if (swap!=i) {
        temp.swapColumn(i,swap);
        this->swapColumn(i,swap);
    }
    t=temp[i*4+i];

    this->scaleColumn(i,1.0/t);
    temp.scaleColumn(i,1.0/t);
    for(k=i+1;k<4;k++) {
        t2=temp[k*4+i];
        this->subScaleColumn(k,i,t2);
        temp.subScaleColumn(k,i,t2);
    }
  }

  // remontée
  for(i=3;i>=0;i--) {
    for(k=i-1;k>=0;k--) {
        t2=temp[i+k*4];
        this->subScaleColumn(k,i,t2);
        temp.subScaleColumn(k,i,t2);
    }
  }
}


const float *Matrix4::fv() const {
    for(int i=0;i<16;i++) {
        mf[i]=m[i];
    }
    return mf;
}

void Matrix4::set(const Quaternion &q) {
  double x=q.x(),y=q.y(),z=q.z(),w=q.w();
  double x2=x*x,y2=y*y,z2=z*z,w2=w*w;
  double xy=2.0*x*y,xz=2.0*x*z,xw=2.0*x*w,yz=2.0*y*z,yw=2.0*y*w,zw=2.0*z*w;
  this->setColumn(0,w2+x2-y2-z2,xy+zw,xz-yw,0);
  this->setColumn(1,xy-zw,w2-x2+y2-z2,yz+xw,0);
  this->setColumn(2,xz+yw,yz-xw,w2-x2-y2+z2,0);
  this->setColumn(3,0,0,0,1);
}

void Matrix4::setRotation(const Vector3 &v1,const Vector3 &v2) {
    Quaternion q;
    q.set(v1,v2);
    this->set(q);
}

void Matrix4::setRotation(float angle,const Vector3 &axis) {
    Quaternion q;
    q.setFromAngleAxis(angle,axis);
    this->set(q);
}

void Matrix4::setScale(double kx,double ky,double kz) {
	setRow(0,kx,0,0,0);
	setRow(1,0,ky,0,0);
	setRow(2,0,0,kz,0);
	setRow(3,0,0,0,1);
}

void Matrix4::setScale(const Vector3 &s) {
	setRow(0,s.x(),0,0,0);
	setRow(1,0,s.y(),0,0);
	setRow(2,0,0,s.z(),0);
	setRow(3,0,0,0,1);
}


void Matrix4::setTranslation(const Vector3 &t) {
    setRow(0,1,0,0,t.x());
    setRow(1,0,1,0,t.y());
    setRow(2,0,0,1,t.z());
    setRow(3,0,0,0,1);
}

const Matrix4 &Matrix4::translation(const Vector3 &v) {
  _compute.setTranslation(v);
  return _compute;
}

const Matrix4 &Matrix4::scaling(const Vector3 &v) {
  _compute.setScale(v);
  return _compute;
}

const Matrix4 &Matrix4::rotation(double angle,const Vector3 &axe) {
  _compute.setRotation(angle,axe);
  return _compute;
}

const Matrix4 &Matrix4::rotation(const Quaternion &q) {
  _compute.set(q);
  return _compute;
}

void Matrix4::scale(const Vector3 &u) {
  Matrix4 m;
  m.setScale(u);
  this->mul(m);
}


Vector4 Matrix4::getRow(unsigned int i) {
	Vector4 res(m[i],m[i+4],m[i+8],m[i+12]);
	return res;
}

void Matrix4::fromModelView() {
  float m[16];
  glGetFloatv(GL_MODELVIEW_MATRIX,m);
  set(m);

}

void Matrix4::translate(const Vector3 &u) {
  Matrix4 m;
  m.setTranslation(u);
  this->mul(m);
}

void Matrix4::rotate(double angle,const Vector3 &u) {
  Matrix4 m;
  m.setRotation(angle,u);
  this->mul(m);
}

void Matrix4::mix(const Matrix4 &m1,const Matrix4 &m2,double t) {
  for(int k=0;k<16;k++) {
    this->set(k,m1.get(k)*(1-t)+m2.get(k)*t);
  }
}

Matrix4 prog3d::operator *(const Matrix4 &m1,const Matrix4 &m2) {
  Matrix4 res;
  res.set(m1);
  res.mul(m2);
  return res;
}

void Matrix4::frustum(double left,double right,double bottom,double top,double near,double far) {
  Matrix4 mat;
  mat.setRow(0,2.0*near/(right-left)  ,0                      ,(right+left)/(right-left)  ,0);
  mat.setRow(1,0                      ,2.0*near/(top-bottom)  ,(top+bottom)/(top-bottom)  ,0);
  mat.setRow(2,0                      ,0                      ,-(far+near)/(far-near)     ,-2.0*far*near/(far-near));
  mat.setRow(3,0                      ,0                      ,-1                         ,0);

  this->mul(mat);
}

void Matrix4::ortho(double left,double right,double bottom,double top,double near,double far) {
  Matrix4 mat;
  mat.setRow(0,2.0/(right-left)  ,0                 ,0                  ,-(right+left)/(right-left));
  mat.setRow(1,0                 ,2.0/(top-bottom)  ,0                  ,-(top+bottom)/(top-bottom));
  mat.setRow(2,0                 ,0                 ,-2.0/(far-near)    ,-(far+near)/(far-near));
  mat.setRow(3,0                 ,0                 ,0                  ,1);

  this->mul(mat);
}


void Matrix4::lookAt(const Vector3 &position,const Vector3 &at,const Vector3 &up) {
  Vector3 i,j,k;

  k.set(position-at);
  i.cross(up,k);

  j.cross(k,i);

  i.normalize();
  j.normalize();
  k.normalize();

  Matrix4 look;
  look.set(position,i,j,k);
  look.invert();

  this->mul(look);
}


void Matrix4::push() {
  double *s,*p;
  s=new double[16];
  p=s;
  for(unsigned int i=0;i<16;i++) {
    *(p++)=m[i];
  }
  _stack.push(s);
}

void Matrix4::pop() {
  double *s;
  s=_stack.top();
  for(unsigned int i=0;i<16;i++) {
    m[i]=s[i];
  }
  _stack.pop();
  delete s;
}

void Matrix4::gl2() {
  glLoadIdentity();
  glMultMatrixd(dv());
}


Matrix3 Matrix4::normalMatrix() {
	Matrix4 res;
	res.set(*this);
	res.invert();
	res.transpose();
	return Matrix3(res);
}





