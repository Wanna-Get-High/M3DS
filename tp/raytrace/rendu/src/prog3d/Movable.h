#ifndef INC_Movable_H
#define INC_Movable_H

/**
@author F. Aubert
*/

#include "Quaternion.h"
#include <stack>
#include "Vector3.h"
#include "Line.h"

namespace prog3d {

  typedef enum {PARENT_COORDINATE,LOCAL_COORDINATE} EMovable;

  class Movable {
    Quaternion _orientation;
    Vector3 _position;
    Vector3 _scale;

    std::stack<Quaternion> _pq;
    std::stack<Vector3> _pp;
    std::stack<Vector3> _ps;


    Quaternion _saveOrientation;
    Vector3 _savePosition;
    Vector3 _saveScale;

    public:
    Movable();

    /// copy constructor
    Movable(const Movable &f);
    virtual ~Movable();

    Matrix4 matrix() const;
    Matrix4 matrixInverse() const;

    void compose(const Movable &f);

    void translate(const Vector3 &tr,EMovable f);
    void rotate(double angle,const Vector3 &axe,EMovable f=PARENT_COORDINATE);
    void rotate(double angle, const Vector3 &axe, EMovable f, const Vector3 &center);
    void rotate(const Quaternion &q,EMovable f);
    void scale(const Vector3 &s,EMovable f);

    void set(const Movable &f);

    void save();
    void restore();

    void setIdentity();

    inline const Vector3 &position() const {return _position;}
    inline const Quaternion &orientation() const {return _orientation;}
    inline const Vector3 &scale() const {return _scale;}

    inline void position(const Vector3 &pos) {_position=pos;}
    inline void orientation(const Quaternion &q) {_orientation=q;}
    inline void scaling(const Vector3 &s) {_scale=s;}
    inline void scaling(double k) {_scale=Vector3(k,k,k);}

    void orientation(const Vector3 &i,const Vector3 &j,const Vector3 &k);
    void orientation(double *a,Vector3 *u);
    void orientation(double a,const Vector3 &u);

    // linear interpolation this=(1-t)f1+tf2 (i.e. linear interpolation of the translation and the quaternion)
    void mix(const Movable &f1,const Movable &f2,double t);

    // returns the point/direction in LOCAL (f=F_LOCAL and u supposed to be given in PARENT) or in PARENT (f=F_PARENT and u supposed to be given in LOCAL)
    Vector3 pointTo(EMovable f,const Vector3 &u);
    void pointTo(EMovable f,Vector3 *u);
    Vector3 directionTo(EMovable f,const Vector3 &u);
    void directionTo(EMovable f,Vector3 *u);

    /// set/get the the z direction (from/to the orientation)
    prog3d::Vector3 direction() const;
    void direction(const Vector3 &u);


    void setOrientationLookat(const Vector3 &pos);
    void setOrientationLookat(const Vector3 &pos,const Vector3 &up);

    void applyGL();

    void push();
    void pop();

    void pushOrientation();
    void popOrientation();

    void transformTo(EMovable f, Line *l);
    Line transformTo(EMovable f, const Line &l);
  };

}

#endif

