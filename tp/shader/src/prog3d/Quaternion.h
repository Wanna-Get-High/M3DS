#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED

#include <iostream>

/**
@file
@author F. Aubert
@brief Quaternion (x,(x,y,z)) operations to represent rotation
*/


namespace prog3d {

class Matrix4;
class Vector3;

/**
@class Quaternion
@brief Quaternion (x,(x,y,z)) operations to represent rotation
*/
class Quaternion {
  double _w,_x,_y,_z;

public:
  /// copy constructor
  Quaternion(const Quaternion &q);

  /// constructor (default = unit quaternion (1,(0,0,0)))
  Quaternion(double w=1.0,double x=0.0,double y=0.0,double z=0.0);
  /// constructor
  Quaternion(double a,const Vector3 &n);

  /// donne w
  inline double w() const {return _w;}
  /// donne x
  inline double x() const {return _x;}
  /// donne y
  inline double y() const {return _y;}
  /// donne z
  inline double z() const {return _z;}


  /// quaternion identit� (neutre pour la multiplication)
  void setIdentity();

  /// affectation pour (w,(x,y,z))
  void set(double w=1.0,double x=0.0,double y=0.0,double z=0.0);
  /// affectation par (v1,v2) (produit vectoriel et demi-angle)
  void set(const Vector3 &v1,const Vector3 &v2);
  /// affectation selon (scalar,u)
  void set(double scalar,const Vector3 &u);
  /// affectation selon angle-axis
  void setFromAngleAxis(double angle,const Vector3 &v);
  /// affecte l'angle et l'axe selon le quaternion (doit �tre une rotation)
  void copyToAngleAxis(double *angle,Vector3 *u) const;

  /// donne la norme du quaternion
  double length() const;
  /// donne le carr� de la norme du quaternion
  double length2();
  /// donne la norme du vecteur du quaternion
  double lengthXYZ() const;
  /// normalise le quaternion
  void normalize();

  /// copy the matrix computed from the quaternion to m
  void copyTo(Matrix4 *m);

  /// affecte le quaternion de rotation selon la matrice src
  void set(const Matrix4 &src);

  /// affecte le quaternion avec le rep�re (i,j,k)
  void set(const Vector3 &i,const Vector3 &j,const Vector3 &k);

  /// multiplie � droite le quaternion par q
  void mul(const Quaternion &q);

  /// multiplie � gauche le quaternion par q
  void mulLeft(const Quaternion &q);

  /// affecte le quaternion avec le produit q1*q2
  void mul(const Quaternion &q1,const Quaternion &q2);
  /// affectate le quaternion avec la recopie de q
  void set(const Quaternion &q);
  /// conjugue le quaternion
  void conjugate();
  /// linear interpolation this=(1-t)q1+tq2 => post normalized
  void mix(const Quaternion &q1, const Quaternion &q2,double t);
  /// affecte le quaternion avec q1+q2
  void add(const Quaternion &q1,const Quaternion &q2);
  /// modifie le quaternion en le multipliant par s
  void scale(double s);
  /// multiply and add (no normalized)
  void mad(double alpha,const Quaternion &q);
  /// add : this=this+q (no post normalized)
  void add(const Quaternion &q);
  /// scale the quaternion : this=s*this
  void mul(double s);

  /// set the quaternion with the rotation angle/axis (same as copyFromAngleAxis)
  void setRotation(double angle,const Vector3 &u);

  /// cumulate with the rotation (angle in degree)
  void rotate(double angle,const Vector3 &u);

  ///
  void transform(Vector3 *u);

  /// donne le produit de q1*q2
  friend Quaternion operator*(const Quaternion &q1,const Quaternion &q2);
  /// donne k*q
  friend Quaternion operator *(double k,const Quaternion &q);

  /// returns the transformation of u by q (i.e. conjugate(q)*u*q).
  friend Vector3 operator *(const Quaternion &q,const Vector3 &u);

  /// compatibilit� avec cout
  friend std::ostream& operator<<(std::ostream &s,const Quaternion &q);

  friend Quaternion conjugate(const Quaternion &q);
  friend Quaternion normalize(const Quaternion &q);

  static Quaternion identity();
  static Quaternion fromAngleAxis(double angle, const Vector3 &v);
  static Quaternion fromMatrix(const Matrix4 &m);
};

// declaration of the friend functions
Quaternion operator*(const Quaternion &q1,const Quaternion &q2);
Quaternion operator+(const Quaternion &q1,const Quaternion &q2);
Quaternion operator *(double k,const Quaternion &q);
Quaternion operator *(const Quaternion &q,double k);
std::ostream& operator<<(std::ostream &s,const Quaternion &q);
Quaternion conjugate(const Quaternion &q);
Quaternion normalize(const Quaternion &q);
Quaternion quaternion(const Matrix4 &m);
}

#endif // QUATERNION_H_INCLUDED

