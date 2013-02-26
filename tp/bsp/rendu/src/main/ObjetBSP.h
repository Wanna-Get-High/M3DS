#ifndef INC_OBJETBSP_H
#define INC_OBJETBSP_H

#include "MeshObj.h"
#include "ArbreBSP.h"
#include "FaceBSP.h"

class ObjetBSP {
private:
  ArbreBSP *_bsp;

  std::vector<FaceBSP *> _allFace;
  std::vector<VertexBSP *> _allVertex;

public:
  ObjetBSP();
  virtual ~ObjetBSP();

  void createBSP();
  ArbreBSP *consBSP(const std::vector<FaceBSP *> &listeFace);
  void drawBSP(ArbreBSP *tree,const prog3d::Vector3 &p);

  void drawBSP(const prog3d::Vector3 &eye);

  FaceBSP *createFace();
  VertexBSP *createVertex(const prog3d::Vector3 &p);

  inline FaceBSP *face(unsigned int i) {return _allFace[i];}

  void add(VertexBSP *v);
  void add(FaceBSP *f);
  void suppress(FaceBSP *f);

  void drawDepth();
  void drawBrut();
  void drawBSP();
  void drawGrid();

  void read(const std::string &name);

  inline unsigned int nbFace() {return _allFace.size();}
  inline unsigned int nbVertex() {return _allVertex.size();}
  inline ArbreBSP *bsp() {return _bsp;}
};

#endif
