#include "glsupport.h"
#include "ViewportUtil.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Error.h"
#include <iostream>

/*


 F. Aubert

*/
using namespace prog3d;
using namespace std;


ViewportUtil::ViewportUtil() {
    localWindow.setIdentity();
    normalizedLocal.setIdentity();
    windowNormalized.setIdentity();
    normalizedWindow.setIdentity();
    eyeLocal.setIdentity();
}

ViewportUtil::~ViewportUtil() {}

	// transform the mouse (Window coordinates) to the local coordinates (accordingly to localScreen matrix)
	// reference (local coordinates) is required to set the depth of the mouse cursor in screen
Vector3 ViewportUtil::applyLocalWindow(const Vector2 &mouse,const Vector3 &reference) {
		Vector4 ref4d(reference);
		normalizedLocal.transform(&ref4d);
		Vector4 result4d(mouse.x(),mouse.y(),ref4d.z()/ref4d.w(),1.0);
		localWindow.transform(&result4d);
		Vector3 result;
		result.point(result4d);
		return result;
	}

// CU : depth should be comprised between [-1,1] (depth in normalized device coordinate)
Vector3 ViewportUtil::applyLocalWindow(const Vector2 &mouse,double depth) {
	Vector4 result4d(mouse.x(),mouse.y(),depth,1);
//	result4d.print("result4d avant =");
	localWindow.transform(&result4d);
//	localWindow.print("localWindow =");
//	result4d.print("result4d =");
	Vector3 result;
	result.point(result4d);
	return result;
}

Vector3 ViewportUtil::applyNormalizedLocal(const Vector3 &p) const {
  Vector3 result;
  result=normalizedLocal.transformPoint(p);
  return result;
}


Vector3 ViewportUtil::applyEyeLocal(const Vector3 &p) const {
    Vector3 res;
    res.set(p);
    eyeLocal.transformPoint(&res);
    return res;
}

Vector2 ViewportUtil::applyWindowLocal(const Vector3 &p) const {
    return this->windowCoordinate(p);
}


void ViewportUtil::setView() {
    // passage souris en normalisé : xn=1/wv*xs-xv, yn=1/yw*ys-yv
	Matrix4 mproj;
	Matrix4 mmodelview;
	Matrix4 localNormalized;
	double viewport[4]; // viewport : [0]=x_min,[1]=y_min, [2]=width, [3]=height
	double mat[16];
	glGetDoublev(GL_VIEWPORT, viewport);
	glGetDoublev(GL_PROJECTION_MATRIX,mat);
	mproj.set(mat);
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	eyeLocal.set(mat);
	mmodelview.set(mat);
	normalizedLocal.mul(mproj,mmodelview);
	localNormalized.invert(normalizedLocal);


	normalizedWindow.setRow(0,2.0/viewport[2],0,0,-1-viewport[0]);
	normalizedWindow.setRow(1,0,-2.0/viewport[3],0,1+viewport[1]);
	normalizedWindow.setRow(2,0,0,1,0);
	normalizedWindow.setRow(3,0,0,0,1);
//	normalizedWindow.print("normalizedWindow");
	windowNormalized.invert(normalizedWindow);
//	windowNormalized.print("windowNormalized");
	localWindow.mul(localNormalized,normalizedWindow);
//	localWindow.print("localWindow=");
}

Vector2 ViewportUtil::windowCoordinate(const Vector3 &p) const {
	Vector2 point2;
	Vector4 point3(p);
	normalizedLocal.transform(&point3);
	windowNormalized.transform(&point3);
	point2.x(point3.x()/point3.w());
	point2.y(point3.y()/point3.w());
	return point2;
}

Vector3 ViewportUtil::cameraLocal() {
	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,mat);
	Matrix4 m;
	m.set(mat);
	m.invert();
	Vector3 p(0,0,0);
	m.transformPoint(&p);
	return p;
}

const Matrix4 &ViewportUtil::getEyeLocal() const {
	return eyeLocal;
}



