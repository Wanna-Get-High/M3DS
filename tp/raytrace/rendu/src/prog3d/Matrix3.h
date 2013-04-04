/*
 * Matrix3.h
 *
 *  Created on: 14 mars 2011
 *      @author: aubert
 */

#ifndef MATRIX3_H_
#define MATRIX3_H_

#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"

namespace prog3d {
  class Matrix4;


class Matrix3 {
  double _c[9];
  static float _cf[9]; ///< for casting in float

public:
  Matrix3();
  virtual ~Matrix3();

  explicit Matrix3(const Matrix4 &m);

  const float *fv() const ;
  inline const double *dv() const {return _c;}
  void set(const prog3d::Matrix3 &m);
  inline double operator[](unsigned int i) const {return _c[i];}

  void transform(prog3d::Vector3 *p) const;


};

Vector3 operator*(const prog3d::Matrix3 &m,const prog3d::Vector3 &p);

}
#endif /* MATRIX3_H_ */

