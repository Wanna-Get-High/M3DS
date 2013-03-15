#include "glsupport.h"
#include "Trackball.h"
#include "Vector3.h"
#include <math.h>
#include "UtilGL.h"

using namespace prog3d;
using namespace std;

Trackball::Trackball() {
  _orientation.setIdentity();
  _orientationX.setIdentity();
  _orientationY.setIdentity();
  _started=false;
}

void Trackball::setIdentity() {
	_orientation.setIdentity();
	_orientationX.setIdentity();
	_orientationY.setIdentity();
}

void Trackball::start(int x,int y) {
  _started=true;

  _xStart=x;
  _yStart=y;
}

void Trackball::stop() {
  _started=false;
}

void Trackball::motion(int x,int y,const Quaternion &q) {
  if (_started) {
  double angleY=x-_xStart;
  double angleX=y-_yStart;
  Quaternion q1;
  q1.setIdentity();
  q1.setFromAngleAxis(angleX,q*Vector3(1,0,0));
  _orientation=q1*_orientation;
  q1.setFromAngleAxis(angleY,q*Vector3(0,1,0));
  _orientation=q1*_orientation;
  _orientation.normalize();
  //
  _xStart=x;
  _yStart=y;
  }
}

void Trackball::motionXY(int x,int y) {
  if (_started) {
  double angleY=x-_xStart;
  double angleX=y-_yStart;
  Quaternion q1;
  q1.setIdentity();
  q1.setFromAngleAxis(angleX,Vector3(1,0,0));
  _orientationY=q1*_orientationY;
  q1.setFromAngleAxis(angleY,Vector3(0,1,0));
  _orientationX=q1*_orientationX;
  _orientation=_orientationY*_orientationX;
  _orientation.normalize();
  //
  _xStart=x;
  _yStart=y;
  }
}


bool Trackball::isStarted() {
  return _started;
}

void Trackball::applyGL() {
  Vector3 u;
  double angle;
  _orientation.copyToAngleAxis(&angle,&u);
  glRotatef(angle,u.x(),u.y(),u.z());

}



