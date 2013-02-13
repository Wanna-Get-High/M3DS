/**
Objet représenté sous forme BSP
(container : liste de faces, sommets, arbre)

@author Fabrice Aubert
*/


#include "ObjetBSP.h"
#include "FaceBSP.h"
#include <iostream>
#include <algorithm>
#include "Error.h"
#include "UtilGL.h"
#include "MeshObj.h"

using namespace prog3d;
using namespace std;



/// Construction d'un arbre BSP selon une liste de facettes (statique et récursif)
ArbreBSP *ObjetBSP::consBSP(const vector<FaceBSP *> &tabFace) {
  /// Il s'agit de prendre un pivot dans tabFace, et de séparer l'ensemble des autres faces dans listeNegative et listePositive.
  /// puis on récursive en construisant l'arbre.
  /// Les setters de ArbreBSP sont node(une_face), left(un_arbre), right(un_arbre)
  /// Après un appel à une_face1->separate(une_face2), vous récupérez les faces qui y sont construites par
  ///     une_face1->facePositive() et une_face1->faceNegative() vous donneront les faces (éventuellement NULL)
  /// Remarque : l'appel initial de cette récursivité est fait par la méthode ObjetBSP::createBSP()

  ArbreBSP *res=NULL;
  vector<FaceBSP *> listeNegative,listePositive;
  listeNegative.clear();
  listePositive.clear();

  if (tabFace.empty()) {
    return NULL;
  } else {
    res=new ArbreBSP();
    // TODO : A COMPLETER (construire listeNegative et listePositive; mettre le pivot comme noeud de l'arbre res)

    // appels récursifs
    res->left(consBSP(listeNegative));
    res->right(consBSP(listePositive));
    return res;
  }
}


/** **************************************************************** */
/** Affichage de l'arbre BSP par le peintre */

/// méthode statique récursive (appel initial fait par ObjetBSP::drawBSP(Const Vector3 &))
void ObjetBSP::drawBSP(ArbreBSP *tree,const Vector3 &eye) {
  /// les sélecteurs sont tree->node() (de type FaceBSP *), et tree->left(), tree->right() (de type ArbreBSP *)
  /// pour tracer une face de type FaceBSP * : une_face->draw()

  // TODO : à compléter
}

/** **************************************************************************** */
/** **************************************************************************** */
/** **************************************************************************** */
/** **************************************************************************** */
/** **************************************************************************** */

/// construction de l'arbre BSP (i.e. construction de l'attribut _bsp) : initialisation de la récursivité
/// appel à consBSP.
void ObjetBSP::createBSP() {
  if (!_bsp) delete _bsp;

  // copier toutes les faces de l'objet dans la liste initiale.
  vector<FaceBSP *> listeFace;
  listeFace.clear();
  for(vector<FaceBSP *>::iterator i=_allFace.begin(); i!=_allFace.end(); i++) {
    listeFace.push_back(*i);
  }

  _bsp=ObjetBSP::consBSP(listeFace);

  // A LAISSER ! On efface toutes les faces de la liste initiale (sauf pivot d'indice 0)
  vector<FaceBSP *>::iterator i=_allFace.begin();
  while(i!=_allFace.end()) {
    if ((*i)->isDelete()) suppress(*i);
    else i++;
  }

}


/// Affichage par peintre : init récursivité.
void ObjetBSP::drawBSP(const Vector3 &eye) {
  glDepthFunc(GL_ALWAYS);
  UtilGL::materialBlueGreen();
  drawBSP(_bsp,eye);
}

/** **************************************************************************** */





ObjetBSP::ObjetBSP() {
  _allFace.clear();
  _allVertex.clear();
  _bsp=NULL;
}

FaceBSP *ObjetBSP::createFace() {
  FaceBSP *res;
  res=new FaceBSP(this);
  return res;
}

VertexBSP *ObjetBSP::createVertex(const Vector3 &p) {
  VertexBSP *res;
  res=new VertexBSP(this);
  res->point(p);
  return res;
}



ObjetBSP::~ObjetBSP() {
  for(vector<FaceBSP *>::iterator i=_allFace.begin(); i<_allFace.end(); i++) {
    delete *i;
  }
  for(vector<VertexBSP *>::iterator i=_allVertex.begin(); i<_allVertex.end(); i++) {
    delete *i;
  }
  delete _bsp;
}


void ObjetBSP::read(const string &name) {
  MeshObj *o=new MeshObj();
  o->read(name);
  o->scaleInBoxMin(-1.2,0.8,-1,1,-1,1);
  o->computeNormal();

  for(unsigned int i=0; i<o->nbVertex(); i++) {
    VertexBSP *v=createVertex(o->vertex(i));
    v->normal(o->normalVertex(i));
  }
  for(unsigned int i=0; i<o->nbFace(); i++) {
    FaceBSP *f=createFace();
    f->normal(o->normalFace(i));
    for(unsigned int j=0; j<o->nbVertexFace(i); j++) {
      f->add(_allVertex[o->indexVertex(i,j)]);
    }
  }
}

void ObjetBSP::suppress(FaceBSP *f) {
  vector<FaceBSP *>::iterator a_sup=find(_allFace.begin(),_allFace.end(),f);
  if (a_sup!=_allFace.end()) {
    delete *a_sup;
    _allFace.erase(a_sup);
  }
  else throw Error("suppress a Face ???",__LINE__,__FILE__);
}

void ObjetBSP::add(FaceBSP *f) {
  _allFace.push_back(f);
}

void ObjetBSP::add(VertexBSP *v) {
  _allVertex.push_back(v);
}

void ObjetBSP::drawDepth() {
  UtilGL::materialFrontBack();
  glDepthFunc(GL_LESS);

  drawGrid();

}

void ObjetBSP::drawGrid() {
  glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);

  for(unsigned int i=0; i<_allFace.size(); i++) {
    _allFace[i]->draw();
  }


  glDisable(GL_POLYGON_OFFSET_FILL);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDisable(GL_LIGHTING);
  glColor3f(0.0,0.0,0.0);
  glLineWidth(2.0);


  for(unsigned int i=0; i<_allFace.size(); i++) {
    _allFace[i]->draw();
  }


  glLineWidth(1.0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glPopAttrib();
}


void ObjetBSP::drawBrut() {
  UtilGL::materialFrontBack();
  glDepthFunc(GL_ALWAYS);

  drawGrid();
}




