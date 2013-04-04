#include "Camera.h"
#include "glsupport.h"
#include <iostream>

#include "Matrix4.h"
#include "Trackball.h"

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

Camera::Camera() {
  frustum(-1,1,-1,1,0.1,1000);
}

Camera::~Camera() {
}


void Camera::frustum(double left,double right,double down,double top,double pnear,double pfar) {

  _frustum[0]=left;
  _frustum[1]=right;
  _frustum[2]=down;
  _frustum[3]=top;
  _frustum[4]=pnear;
  _frustum[5]=pfar;
}

void Camera::viewport(unsigned int x,unsigned int y,unsigned int width,unsigned int height) {
  _viewport[0]=x;
  _viewport[1]=y;
  _viewport[2]=width;
  _viewport[3]=height;
}

bool Camera::mouseToCamera(const Vector2 &u,Vector3 *mouseCam) {
  if ((u.x()<double(_viewport[0])) || (u.x()>double(_viewport[0]+_viewport[2])) || (u.y()<double(_viewport[1])) || (u.y()>double(_viewport[1]+_viewport[3]))) {
    //*mouseCam=Vector3(0,0,0);
    return false;
  }
  else {
    double xWindow=double(u.x()-_viewport[0])/_viewport[2];
    double yWindow=1.0-double(u.y()-_viewport[1])/_viewport[3];
    mouseCam->x(xWindow*(_frustum[1]-_frustum[0])+_frustum[0]);
    mouseCam->y(yWindow*(_frustum[3]-_frustum[2])+_frustum[2]);
    mouseCam->z(-_frustum[4]);
    return true;
  }
}


Matrix4 Camera::cameraToWorld() {
  Matrix4 result;
  result=matrix();
  result.invert();
  return result;
}

Matrix4 Camera::worldToCamera() {
  return matrix();
}

void Camera::applyViewport() {
  glViewport(_viewport[0],_viewport[1],_viewport[2],_viewport[3]);
}


void Camera::applyFrustum() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(_frustum[0],_frustum[1],_frustum[2],_frustum[3],_frustum[4],_frustum[5]);
  glMatrixMode(GL_MODELVIEW);
}

void Camera::applyGL() {
  glMultMatrixd(cameraToWorld().dv());
}



void Camera::forward(double k) {
  translate(Vector3(0,0,-k),LOCAL_COORDINATE);
}

void Camera::backward(double k) {
  translate(Vector3(0,0,k),LOCAL_COORDINATE);
}

void Camera::left(double k) {
  translate(Vector3(-k,0,0),LOCAL_COORDINATE);
}

void Camera::right(double k) {
  translate(Vector3(k,0,0),LOCAL_COORDINATE);
}



void Camera::setViewport(int x,int y,int w,int h) {
  _x=x;_y=y;_w=w;_h=h;
}


void Camera::lookAt(const Vector3 &at,const Vector3 &up) {
  Vector3 i,j,k;

  k.set(position()-at);
  i.cross(up,k);

  j.cross(k,i);

  i.normalize();
  j.normalize();
  k.normalize();

  Quaternion q;
  q.set(i,j,k);
  orientation(q);
}

bool Camera::mouseToRayWorld(const Vector2 &mouse,Line *l) {
  bool ok;
  Vector3 mouseCam;
  ok=mouseToCamera(mouse,&mouseCam);
  if (!ok) return false;
  else {
    Vector3 a(0,0,0);
    Vector3 u=mouseCam;
    pointTo(PARENT_COORDINATE,&a);
    directionTo(PARENT_COORDINATE,&u);
    l->set(a,u);
    return true;
  }
}

bool Camera::mouseToWorld(const Vector2 &mouse,Vector3 *l) {
  bool ok;
  Vector3 mouseCam;
  ok=mouseToCamera(mouse,&mouseCam);
  if (!ok) return false;
  else {
    *l=mouseCam;
    pointTo(PARENT_COORDINATE,l);
    return true;
  }
}



