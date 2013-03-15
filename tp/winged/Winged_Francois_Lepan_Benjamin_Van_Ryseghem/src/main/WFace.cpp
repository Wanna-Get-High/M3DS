#include "WFace.h"
#include "Error.h"
#include "glsupport.h"
#include "UtilGL.h"

/**
@file
@author F. Aubert
@brief  Winged-Edges : opérations sur les sommets

*/


using namespace prog3d;
using namespace std;

/** ************************************************************************************
** A COMPLETER : WFace::draw() => afficher la face par un glBegin(GL_POLYGON); glEnd();
** On parcourt tous les sommets de la face dans l'ordre direct
** - this->edge() : donne l'arête affectée à la face
** - si e de type WEdge * :
**    - e->begin(), e->end() : donne les sommets de l'arête (de type WVertex *)
**    - e->right(), e->left() : donne les faces gauche et droite (de type WFace *) (Remarque : e->left()==this est un test valide)
**    - e->succLeft(), e->succRight(), e->predLeft(), e->predRight() donnent les arêtes incidentes à droite et à gauche (de type WEdge *)
** Pour afficher en OpenGL : si v est de type WVertex *
**     - v->point() donne les coordonnées du sommet v de type Vector3 et un_vector3.dv() donne le tableau des 3 doubles du vecteur (v->point().dv() peut directement être passé à glVertex3dv).
** *********************************************************************************** */
void WFace::draw() {
    glBegin(GL_POLYGON);

    WEdge* edge = this->edge();
    WVertex * begin = edge->begin();
    WVertex * end = edge->end();

    glNormal3dv(begin->normal().dv());
    glVertex3dv(begin->point().dv());

    glNormal3dv(end->normal().dv());
    glVertex3dv(end->point().dv());

    do {
        // on ne sait pas pour cette face si elle se trouve a droite du segment ou a gauche
        if (edge->left() == this) {
            end = edge->end();
            glNormal3dv(end->normal().dv());
            glVertex3dv(end->point().dv());
            edge = edge->succLeft();
        } else {
            begin = edge->begin();
            glNormal3dv(begin->normal().dv());
            glVertex3dv(begin->point().dv());
            edge = edge->succRight();
        }

    }  while (edge != this->edge());

    glEnd();
}

/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/


/** ************************************************************************************
** A COMPLETER : WFace::computePointFace() => point de face de catmull clark (iso barycentre des sommets).
** rappel : les coordonnés d'un sommet WVertex *v s'obtiennent par v->point() (de type Vector3).
** ************************************************************************************ */
void WFace::computePointFace() {
  // todo : à enlever
  WEdge *e=edge();
  Vector3 barycentre;
  WVertex *v;
  int nbedge=0;
  barycentre.set(0,0,0);
  do {
    if (e->left()==this) v=e->begin(); else v=e->end();
    barycentre+=v->point();
    nbedge++;
    if (e->left()==this) e=e->succLeft(); else e=e->succRight();
  } while (e!=edge());
  barycentre=barycentre/nbedge;

  // affectation point de face
  _pointFace.set(barycentre);
}


/** ************************************************************************************
** A COMPLETER : WFace::drawLineCatmull() => on trace tous les segments qui relie le point de face aux points d'arête selon Catmull-Clark
** - this->pointFace() donne le Vector3 du point de face
** - il faut parcourir toutes les arêtes e appartenant à this, et e->pointEdge() donne alors le point d'arête (de type Vector3).
** - la procédure UtilGL::draw(p1,p2) trace le segment entre p1 et p2 (de type Vector3).
** *********************************************************************************** */

void WFace::drawLineCatmull() {
  // TODO : à virer
  WEdge *e=this->edge();
  do {
    UtilGL::draw(this->pointFace(),e->pointEdge());
    if (this==e->left())
      e=e->succLeft();
    else
      e=e->succRight();
  } while (e!=this->edge());
}


/** *********************************************************************************** */




WFace::WFace(Winged *owner) {
  _owner=owner;
  _edge=NULL;
  //ctor
}

WFace::~WFace() {
  //dtor
}

void WFace::edge(WEdge *e) {
  _edge=e;
}

WEdge *WFace::edge() {
  return _edge;
}

Winged *WFace::owner() {
  return _owner;
}

void WFace::index(unsigned int i) {
  _index=i;
}

unsigned int WFace::index() {
  return _index;
}

void WFace::normal(const Vector3 &n) {
  _normal.set(n);
}

const Vector3 &WFace::normal() const {
  return _normal;
}

const Vector3 &WFace::pointFace() const {
  return _pointFace;
}


void WFace::computeNormal() {
  WEdge *u,*v,*start;
  Vector3 uvect,vvect;
  Vector3 n;
  start=edge();
  u=edge();
  if (u->left()==this) uvect=u->direction(); else uvect=-u->direction();
  do {
    if (u->left()==this) {
      v=u->succLeft();
    }
    else {
      v=u->succRight();
    }
    vvect=v->direction();
    if (v->right()==this) vvect=-vvect;
    n=uvect.cross(vvect);
    u=v;
    uvect=vvect;
  } while (n.length2()<0.000000001 && (u!=start));
  if (u==start) {
    normal(Vector3(0,0,0)); // cas particulier : face aplatie
  }
  else {
    n.normalize();
    normal(n);
  }
}


void WFace::insertVertexPointFace() {
  WVertex *new_vertex=owner()->createVertex(pointFace());

  WEdge *start_new,*new_edge,*pred_edge;
  WEdge *start=edge();
  if (start->left()==this) start=start->succLeft();

  WFace *new_face;

  WEdge *e1=start,*e2,*succ_e2;
  if (e1->left()==this) {
    e2=e1->succLeft();
    new_edge=_owner->createEdge(new_vertex,e1->begin());
  }
  else {
    e2=e1->succRight();
    new_edge=_owner->createEdge(new_vertex,e1->end());
  }
  new_vertex->edge(new_edge);
  pred_edge=new_edge;
  start_new=new_edge;

  do {
    new_face=_owner->createFace();
    new_face->edge(e1);
    if (e2->left()==this) {
      succ_e2=e2->succLeft();
      if (succ_e2!=start) new_edge=_owner->createEdge(new_vertex,e2->end()); else new_edge=start_new;
      e2->left(new_face);
      e2->succLeft(new_edge);
    }
    else {
      succ_e2=e2->succRight();
      if (succ_e2!=start) new_edge=_owner->createEdge(new_vertex,e2->begin()); else new_edge=start_new;
      e2->succRight(new_edge);
      e2->right(new_face);
    }

    new_edge->right(new_face);
    new_edge->succRight(pred_edge);
    new_edge->predRight(e2);

    pred_edge->left(new_face);
    pred_edge->predLeft(new_edge);
    pred_edge->succLeft(e1);

    if (e1->left()==this) {
      e1->left(new_face);
      e1->predLeft(pred_edge);
    }
    else {
      e1->right(new_face);
      e1->predRight(pred_edge);
    }

    e1=succ_e2;
    if (e1->left()==this) e2=e1->succLeft(); else e2=e1->succRight();
    pred_edge=new_edge;
  } while (e1!=start);

}



