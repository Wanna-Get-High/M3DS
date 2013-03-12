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

