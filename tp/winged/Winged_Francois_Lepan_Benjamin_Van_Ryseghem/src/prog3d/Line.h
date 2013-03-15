#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

namespace prog3d {
  class Line;
}

#include "Vector3.h"
#include "Plane.h"
#include <string>
#include <ostream>

/*!
*
* @file
*
* @brief Line representation \f$P=A+\lambda u\f$
* @author F. Aubert
*
*/

namespace prog3d {
/**
@class Line
@brief Line representation \f$P=A+\lambda u\f$
*/
class Line {
  Vector3 _u; ///< direction of the line
  Vector3 _a; ///< origin of the line

	public:

  /// destructor
  ~Line();
  /// constructor
  Line();
  Line(const Line &l);
  /// constructor from (origin,direction)
	Line(const Vector3 &aa,const Vector3 &uu);

  /// get origin
  const Vector3 &getA() const{return _a;}
  const Vector3 &point() const {return _a;}
  const Vector3 &origin() const {return _a;}
  const Vector3 &a() const {return _a;}
  /// get direction
  const Vector3 &getU() const {return _u;}
  const Vector3 &direction() const {return _u;}
  const Vector3 &u() const {return _u;}

  /// get the point on the line : a+ku
  Vector3 point(double k) const;


  /// set the origin
  void setA(const Vector3 &aa) {_a=aa;}
  void point(const Vector3 &aa) {_a=aa;}
  void origin(const Vector3 &aa) {_a=aa;}
	/// set the direction
  void setU(const Vector3 &uu) {_u=uu;}
  void direction(const Vector3 &uu)  {_u=uu;}

  /// set the origin and the direction
	void set(const Vector3 &aa,const Vector3 &uu);

  /// set with the intersection of the 2 planes
  bool set(const Plane &p1,const Plane &p2);

  /// squared distance from the point m to the line
  double distance2(const Vector3 &m) const;
  /// print the line
	void print(std::string mesg) const;
	/// Ex : cout << droite << endl; (print (A=...,U=...))
	friend std::ostream& operator<<(std::ostream &s,const Line &l);


};
}

#endif // LINE_H_INCLUDED

