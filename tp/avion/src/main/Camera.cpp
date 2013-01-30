#include "Camera.h"
#include "glsupport.h"
#include <iostream>
#include <cmath>

using namespace prog3d;
using namespace std;

// *********************************************************
// méthodes à compléter lors du TP
void Camera::interpolate(const Camera &cam1,const Camera &cam2,double t) {
}

void Camera::applyGL() {
  Vector3 u;
  double a;
  _orientation.copyToAngleAxis(&a,&u); // conversion quaternion vers (angle,axe) : la variable u est affectée avec l'axe de rotation et a ffectée avec l'angle
}


// **********************************************************
Camera::Camera() {
  //ctor
  _orientation.setIdentity();
  _position.set(0,0,0);
}

Camera::~Camera() {
  //dtor
}

const Quaternion &Camera::orientation() const {
  return _orientation;
}

const Vector3 &Camera::position() const {
  return _position;
}

void Camera::orientation(const Quaternion &q1) {
  _orientation.set(q1);
}

void Camera::position(const Vector3 &p1) {
  _position.set(p1);
}



void Camera::set(const Camera &c) {
  _orientation.set(c.orientation());
  _position.set(c.position());
}


void Camera::lookAt(const Vector3 &at,const Vector3 &up) {
}

