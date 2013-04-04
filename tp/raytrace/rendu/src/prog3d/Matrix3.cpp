/*
 * Matrix3.cpp
 *
 *  Created on: 14 mars 2011
 *      Author: aubert
 */

#include "Matrix3.h"
#include "Matrix4.h"

using namespace prog3d;

float Matrix3::_cf[9];

Matrix3::Matrix3() {
	// TODO Auto-generated constructor stub

}

Matrix3::~Matrix3() {
	// TODO Auto-generated destructor stub
}

const float *Matrix3::fv() const {
    for(unsigned int i=0;i<9;i++) {
        _cf[i]=_c[i];
    }
    return _cf;
}


Matrix3::Matrix3(const Matrix4 &m) {
	_c[0]=m[0];
	_c[1]=m[1];
	_c[2]=m[2];

	_c[3]=m[4];
	_c[4]=m[5];
	_c[5]=m[6];

	_c[6]=m[8];
	_c[7]=m[9];
	_c[8]=m[10];
}

void Matrix3::set(const prog3d::Matrix3 &m) {
  for(unsigned int i=0;i<9;i++) {
    _c[i]=m[i];
  }
}

void Matrix3::transform(Vector3 *p) const {
  Vector3 temp(*p);
  p->x(_c[0]*temp.x()+_c[3]*temp.y()+_c[6]*temp.z());
  p->y(_c[1]*temp.x()+_c[4]*temp.y()+_c[7]*temp.z());
  p->z(_c[2]*temp.x()+_c[5]*temp.y()+_c[8]*temp.z());
}

Vector3 prog3d::operator*(const prog3d::Matrix3 &m,const prog3d::Vector3 &p) {
  Vector3 res(p);
  m.transform(&res);
  return res;
}

