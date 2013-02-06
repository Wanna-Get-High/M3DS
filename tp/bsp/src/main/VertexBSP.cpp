#include "VertexBSP.h"
#include "ObjetBSP.h"
#include "Vector3.h"

using namespace prog3d;

VertexBSP::VertexBSP(ObjetBSP *owner) {
  _owner=owner;
  _owner->add(this);
}

VertexBSP::~VertexBSP() {
}

void VertexBSP::interpolateNormal(const VertexBSP &v1,const VertexBSP &v2) {
  double k=Vector3(v1.point(),this->point()).length()/Vector3(v1.point(),v2.point()).length();
  this->_normal=(1.0-k)*v1.normal()+k*v2.normal();
}
