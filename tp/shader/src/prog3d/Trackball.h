#ifndef INC_TRACKBALL_H
#define INC_TRACKBALL_H

/**
  @file
  @author F. Aubert
  @brief basic trackball
  **/
#include "Quaternion.h"

namespace prog3d {


class Trackball {
  Quaternion _orientation,_orientationX,_orientationY;

  bool _started;
  int _xStart,_yStart;
  int cx,cy;
public:
  /// ctor
  Trackball();
  /// @brief return the orientation
  inline const Quaternion &quaternion() {return _orientation;}
  /// @brief start the tracking (save pointer start)
  void start(int x,int y);
  /// @brief stop the tracking
  void stop();
  /// @brief computes the current orientation (in the frame q)
  void motion(int x,int y,const Quaternion &q=Quaternion::identity());
  void motionXY(int x,int y);
  /// @brief is the tracking active ?
  bool isStarted();
  /// @brief apply the orientation to the OpenGL modelview
  void applyGL();
  /// @brief reset the trackball
  void setIdentity();

};

}

#endif

