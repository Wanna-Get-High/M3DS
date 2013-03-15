#ifndef WVERTEX_H
#define WVERTEX_H

class WVertex;

/**
@file
@author F. Aubert
@brief  Winged-Edges : opérations sur les sommets

*/

#include "Vector3.h"
#include "WEdge.h"
#include "Winged.h"

class WVertex
{
  public:
    virtual ~WVertex();

    WVertex(const prog3d::Vector3 &p,Winged *owner);

    Winged *owner();

    WEdge *edge();
    void edge(WEdge *e);

    const prog3d::Vector3 &normal() const;
    void normal(const prog3d::Vector3 &n);

    const prog3d::Vector3 &point() const;
    void point(const prog3d::Vector3 &p);

    unsigned int index();
    void index(unsigned int i);

    WEdge *indirectBoundary();
    WEdge *directBoundary();

    void computeNormal();
    void computePointVertex();

    const prog3d::Vector3 &pointVertex() const;

    void drawLineCatmull();

    void checkVertexBound();

  protected:
  private:
  prog3d::Vector3 _p;
  prog3d::Vector3 _normal;

  prog3d::Vector3 _pointCatmull;

  WEdge *_edge;

  Winged *_owner;
  unsigned int _index;
};

#endif // WVERTEX_H

