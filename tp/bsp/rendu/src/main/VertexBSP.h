#ifndef INC_VERTEXBSP_H
#define INC_VERTEXBSP_H

#include "Vector3.h"

class ObjetBSP;

class VertexBSP {
  prog3d::Vector3 _point;
  prog3d::Vector3 _normal;

  ObjetBSP *_owner;

public:
  VertexBSP(ObjetBSP *owner);
  virtual ~VertexBSP();

  inline void point(const prog3d::Vector3 &p) {_point.set(p);}
  inline void normal(const prog3d::Vector3 &n) {_normal.set(n);}

  inline const prog3d::Vector3 &point() const {return _point;}
  inline const prog3d::Vector3 &normal() const {return _normal;}
  void interpolateNormal(const VertexBSP &v1, const VertexBSP &v2);
};


#endif
