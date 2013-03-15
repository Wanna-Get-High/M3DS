#ifndef WINGED_H
#define WINGED_H


/**
@file
@author F. Aubert
@brief  Winged-Edges : stockage et construteurs des entités d'un winged + opérations sur l'objet

*/


#include "Vector3.h"
#include <string>

#include "WVertex.h"
#include "WEdge.h"
#include "WFace.h"
#include "Color.h"

class Winged
{
  public:
    Winged();
    virtual ~Winged();

    void read(std::string nom);

    unsigned int nbVertex();
    unsigned int nbFace();
    unsigned int nbEdge();

    void eraseFace(unsigned int i);
    void deleteFace(unsigned int i);

    WVertex *createVertex(const prog3d::Vector3 &p);
    WFace *createFace();
    WEdge *createEdge(WVertex *v1,WVertex *v2);

    Winged *clone();

    WEdge *findEdge2(WVertex *v1,WVertex *v2);

    WVertex *vertex(int i);
    WFace *face(int i);
    WEdge *edge(int i);

    void index();

    void stats();


    void computeNormalFace();
    void computeNormalVertex();


    // methods for catmull-clark subdivision scheme
    void computePointFace();
    void computePointEdge();
    void computePointVertex();
    void catmullClark();

    void check();
    void checkVertexBound();
    void checkEdgeFree();

    void draw();
    void drawEdgeBoundary();
    void drawPointFace();
    void drawPointEdge();
    void drawPointVertex();
    void drawWireHide(const prog3d::Color &color);
    void drawWire(const prog3d::Color &color);
    void drawEdgeBound(const prog3d::Color &color);
    void drawLineCatmull();

    void markAll();
    void unmarkAll();

    std::vector<WEdge *> boundary();

  protected:
  private:
  std::vector<WEdge *> _edge;
  std::vector<WFace *> _face;
  std::vector<WVertex *> _vertex;
};

#endif // WINGED_H

