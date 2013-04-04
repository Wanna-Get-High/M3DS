#ifndef INC_CAMERA_H
#define INC_CAMERA_H

#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Trackball.h"
#include "Movable.h"
#include <stack>
#include "Line.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

namespace prog3d {

class Camera : public Movable {
  float _x,_y,_w,_h; ///< stores the viewport
  double _frustum[6]; ///< left,right,down,top,near,far
  unsigned int _viewport[4]; ///< x,y,width,height
public:
  /// constructor
  Camera();
  /// destructor
  virtual ~Camera();

  /// returns the matrix camToWorld
  Matrix4 cameraToWorld();
  /// returns the matrix worldToCam
  Matrix4 worldToCamera();

  /// mult openGL modelview by the invert (camToWorld)
  void applyGL();

  /// sets the camera such it looks the point at and the up direction is given by up
  void lookAt(const Vector3 &at,const Vector3 &up);

  /// translates in the direction of the camera
  void forward(double k);
  void backward(double k);

  /// translates in the $x$ direction
  void right(double k);
  void left(double k);

  /// set the viewport (unused)
  void setViewport(int x,int y,int w,int h);

  /// sets with linear interpolation between c1 and c2
  void mix(const Camera &c1,const Camera &c2,double k);

  void frustum(double left, double right, double down, double top, double pnear, double pfar);
  void viewport(unsigned int x,unsigned int y,unsigned int w,unsigned int h);

  bool mouseToCamera(const Vector2 &u, Vector3 *mouseCam);
  void applyViewport();
  void applyFrustum();
  bool mouseToRayWorld(const prog3d::Vector2 &mouse, prog3d::Line *l);
  bool mouseToWorld(const Vector2 &mouse, Vector3 *l);

  double fLeft() {return _frustum[0];}
  double fRight() {return _frustum[1];}
  double fTop() {return _frustum[2];}
  double fBottom() {return _frustum[3];}
  double fNear() {return _frustum[4];}
  double fFar() {return _frustum[5];}

  int viewX() {return _viewport[0];}
  int viewY() {return _viewport[1];}
  int viewWidth() {return _viewport[2];}
  int viewHeight() {return _viewport[3];}



};

}

#endif

