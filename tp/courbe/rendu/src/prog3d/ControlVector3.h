#ifndef CONTROLVECTOR3_H_INCLUDED
#define CONTROLVECTOR3_H_INCLUDED

/*!
*
* @file
*
* @brief Control an array of points
* @author F. Aubert
*
*/

#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "ViewportUtil.h"

namespace prog3d {
/**
@class ControlVector3
@brief allows to control points in an array with mouse (picking/moving/adding/removing)
*/

class ControlVector3 {
  std::vector<Vector3 *> _data;          // array of points that is controlled
  std::vector<Vector3> _backup;      // in case we want to backup the points (undo motion)


  ViewportUtil *_view;                 // contains the tool to transform 2d screen in 3d local
  Vector3 *_selected;           // the point that can be moved by motion
  Vector3 _startingMouse;              // where the mouse is pressed in local coordinate
  bool _isOwner;

public:
  ControlVector3();
  virtual ~ControlVector3();
  void setView();
  void data(std::vector<Vector3> *v);
  void data(std::vector< std::vector<Vector3> > *v);
  inline const Vector3 &point(unsigned int i) {return *(_data[i]);}
  inline unsigned int size() {return _data.size();}
  inline ViewportUtil* view() {return _view;}

  void backup();
  void restore();

  /*
  void insert(const Vector2 &mouse,Vector3 *p);
  void insert(Vector2 mouse,Vector3 *a,Vector3 *b);
  void erase(Vector3 *v);
  */
  Vector3 insertNearest(const Vector2 &mouse,double seuil,int *where);

  void clear();

  unsigned int nearestPoint(const Vector2 &m);
  bool nearestSegment(const Vector2 &m,Vector3 **a,Vector3 **b,double seuil);

  void controlMouse(Vector3 *p,const Vector2 &m);

  void selectNearest(const Vector2 &m,double seuil);

  void controlSelected(const Vector2 &m);
  void controlTranslate(const Vector2 &m);
  void controlScale(const Vector2 &m);

  void startMouse(const Vector2 &m);


  void draw();
  void createData(unsigned int nb=0);
  std::vector<Vector3 *>::iterator indexOf(Vector3 *p);
  void deleteData();
  Vector3 add(const Vector2 &mouse, unsigned *where);
  Vector3 addEnd(const Vector2 &mouse);
  bool nearestSegment(const Vector2 &m, unsigned int *which, double seuil);
  void appendData(std::vector<Vector3> *d);
};


}

#endif
