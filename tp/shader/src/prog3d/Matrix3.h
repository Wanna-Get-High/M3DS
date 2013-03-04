/*
 * Matrix3.h
 *
 *  Created on: 14 mars 2011
 *      @author: aubert
 */

#ifndef MATRIX3_H_
#define MATRIX3_H_

#include "Matrix4.h"

namespace prog3d {
class Matrix3 {
	double _c[9];
    static float _cf[9]; ///< for casting in float

public:
	Matrix3();
	virtual ~Matrix3();

	Matrix3(const Matrix4 &m);

	const float *fv() const ;
	inline const double *dv() const {return _c;}


};
}
#endif /* MATRIX3_H_ */

