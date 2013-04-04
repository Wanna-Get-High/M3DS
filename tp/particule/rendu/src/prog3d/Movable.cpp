#include "Movable.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "glsupport.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace prog3d;
using namespace std;

Movable::Movable() {
  _orientation.setIdentity();
  _position.set(0,0,0);
  _scale.set(1,1,1);
}

Movable::~Movable() {
}

Movable::Movable(const Movable &f) : _orientation(f.orientation()), _position(f.position()), _scale(f.scale()) {
}


Matrix4 Movable::matrix() const {
  Matrix4 result;
  result.setTranslation(_position);
  result.rotate(_orientation);
  result.scale(_scale);
  return result;
}


void Movable::compose(const Movable &f) {
  _position=_position+_orientation*f.position();
  _orientation=_orientation*f.orientation();
  _scale=_scale*f.scale(); // !!!! no scale for this ! (difficult to find the orientation after scale)
}


void Movable::set(const Movable &f) {
  _orientation.set(f.orientation());
  _scale.set(f.scale());
  _position.set(f.position());
}


void Movable::orientation(const Vector3 &i,const Vector3 &j,const Vector3 &k) {
  _orientation.set(i,j,k);
}

void Movable::applyGL() {
  glTranslated(_position.x(),_position.y(),_position.z());
  double angle;
  Vector3 axis;
  orientation(&angle,&axis);
  glRotated(angle,axis.x(),axis.y(),axis.z());
  glScaled(_scale.x(),_scale.y(),_scale.z());
}

Matrix4 Movable::matrixInverse() const {
  Matrix4 result;
  result=matrix();
  result.invert();
  return result;
}

void Movable::translate(const Vector3 &tr,EMovable f){
  if (f==LOCAL_COORDINATE) {
    _position=_position+_orientation*tr;
  }
  else {
    _position=_position+tr;
  }
}

void Movable::rotate(double angle,const Vector3 &axe,EMovable f) {
  if (f==LOCAL_COORDINATE) {
    _orientation.rotate(angle,axe);
  }
  else {
    Quaternion q;
    q.setRotation(angle,axe);
    _orientation.mulLeft(q);
  }
}

void Movable::rotate(double angle,const Vector3 &axe,const Vector3 &center,EMovable f) {
  translate(center,f);
  rotate(angle,axe,f);
  translate(-center,f);
}

void Movable::rotate(const Quaternion &q,EMovable f) {
  if (f==LOCAL_COORDINATE) {
    _orientation.mul(q);
  }
  else {
    _orientation.mulLeft(q);
  }
}


void Movable::push() {
  _pq.push(_orientation);
  _pp.push(_position);
  _ps.push(_scale);
}

void Movable::pop() {
  _orientation.set(_pq.top());
  _position=_pp.top();
  _scale=_ps.top();
  _pq.pop();
  _pp.pop();
  _ps.pop();
}

void Movable::pushOrientation() {
  _pq.push(_orientation);
}

void Movable::popOrientation() {
  _orientation.set(_pq.top());
  _pq.pop();
}


void Movable::setOrientationLookat(const Vector3 &at) {
  Vector3 i,j,k;

  k.set(_position-at);
  i.cross(Vector3(0,1,0),k);
  if (i.length2()<0.0001) {
    i.cross(Vector3(1,0,0),k);
  }

  j.cross(k,i);

  i.normalize();
  j.normalize();
  k.normalize();

  Quaternion q;
  q.set(i,j,k);
  orientation(q);
}

void Movable::setOrientationLookat(const Vector3 &at,const Vector3 &up) {
  Vector3 i,j,k;

  k.set(_position-at);
  i.cross(up,k);

  j.cross(k,i);

  i.normalize();
  j.normalize();
  k.normalize();

  Quaternion q;
  q.set(i,j,k);
  orientation(q);
}

void Movable::setIdentity() {
  _position.set(0,0,0);
  _orientation.set(1,0,0,0);
}

void Movable::save() {
  _savePosition.set(_position);
  _saveOrientation.set(_orientation);
  _saveScale.set(_scale);
}

void Movable::orientation(double *a,Vector3 *u) {
  _orientation.copyToAngleAxis(a,u);
}

void Movable::orientation(double a,const Vector3 &u) {
  _orientation.setFromAngleAxis(a,u);
}


void Movable::restore() {
  _position.set(_savePosition);
  _orientation.set(_saveOrientation);
  _scale.set(_saveScale);
}

// linear interpolation this=(1-t)f1+tf2 (i.e. linear interpolation of the translation and the quaternion)
void Movable::mix(const Movable &f1,const Movable &f2,double t) {
  _position.mix(f1.position(),f2.position(),t);
  _orientation.mix(f1.orientation(),f2.orientation(),t);
  _scale.mix(f1.scale(),f2.scale(),t);
}

Vector3 Movable::pointTo(EMovable f,const Vector3 &p) {
  Vector3 res(p);
  if (f==LOCAL_COORDINATE) {
	    res.sub(_position);
	    res=conjugate(_orientation)*res;
	    res.mul(Vector3(1.0/_scale.x(),1.0/_scale.y(),1.0/_scale.z()));
 }
  else {
    res.mul(_scale);
     res=_orientation*res;
    res+=_position;
  }
  return res;
}

Vector3 Movable::directionTo(EMovable f,const Vector3 &p) {
  Vector3 res(p);
  if (f==LOCAL_COORDINATE) {
	    res=conjugate(_orientation)*res;
	    res.mul(Vector3(1.0/_scale.x(),1.0/_scale.y(),1.0/_scale.z()));
  }
  else {
	    res.mul(_scale);
	    res=_orientation*res;
  }
  return res;
}


void Movable::pointTo(EMovable f,Vector3 *p) {
  if (f==LOCAL_COORDINATE) {
	    p->sub(_position);
	    conjugate(_orientation).transform(p);
	    p->mul(Vector3(1.0/_scale.x(),1.0/_scale.y(),1.0/_scale.z()));
  }
  else {
    p->mul(_scale);
    _orientation.transform(p);
    p->add(_position);
  }
}

void Movable::directionTo(EMovable f,Vector3 *u) {
  if (f==LOCAL_COORDINATE) {
	    conjugate(_orientation).transform(u);
	    u->mul(Vector3(1.0/_scale.x(),1.0/_scale.y(),1.0/_scale.z()));
  }
  else {
    u->mul(_scale);
    _orientation.transform(u);
  }
}

void Movable::transformTo(EMovable f,Line *l) {
  l->point(pointTo(f,l->point()));
  l->direction(directionTo(f,l->direction()));
}


Line Movable::transformTo(EMovable f,const Line &l) {
  Line res=l;
  res.point(pointTo(f,l.point()));
  res.direction(directionTo(f,l.direction()));
  return res;
}


Vector3 Movable::direction() const {
  return _orientation*Vector3(0,0,1);
}

void Movable::direction(const Vector3 &u) {
  _orientation.set(Vector3(0,0,1),u);
}



