#include "FaceBSP.h"
#include "UtilGL.h"
#include "ObjetBSP.h"
#include <math.h>
#include "Error.h"

using namespace prog3d;
using namespace std;

/**
* @author Fabrice Aubert
*/

/** ********************************************************
  Trois méthodes à compléter pour le TP : sign, intersection et separate

*/


/// détermine le coté du point P par rapport à la face (le résultat de type ESign peut être SIGN_MINUS or SIGN_PLUS)
ESign FaceBSP::sign(const Vector3 &p) const {
    /// méthodes utiles :
    /// - _tabVertex[i]->point() : vous donnera les coordonnées (de type Vector3) du i-ième sommet de la face (i commence à 0)
    /// - _normal : vous donnera la normale (de type Vector3) de la face
    /// - v1.dot(v2) : produit scalaire entre 2 Vector3
    /// - Vector3 v(p1,p2) (ou v.setVector(p1,p2)) : construit (ou affecte) v avec p2-p1


    ESign res=SIGN_NONE;

    // on recupère le point du plan le plus proche de p
    int size = _tabVertex.size();
    double closer_dist = _tabVertex[0]->point().dot(p);
    Vector3 closer_point = _tabVertex[0]->point();

    for (int i = 0; i < size ; ++i) {
        Vector3 point = _tabVertex[i]->point();
        double dist = point.dot(p);

        if (dist > closer_dist) {
            closer_dist = dist;
            closer_point = point;
        }
    }

    Vector3 vec(closer_point,p);

    if (vec.dot(_normal) >= 0) {
        res = SIGN_PLUS;
    } else {
        res = SIGN_MINUS;
    }

    return res;
}


/// Donne le point d'intersection avec le segment [p1p2] (précondition p1 et p2 de signes distincts par rapport à this)
Vector3 FaceBSP::intersection(const Vector3 &p1,const Vector3 &p2) const {
    /// il suffit de résoudre AI.n=0 (plan (A,n)) et I=p1+k*p1p2 (droite donnée par (p1,p2))
    /// - point(i) : vous donne les coordonnées (type Vector3) du sommet i de la facette
    /// - normal() : vous donne la normale de la facette (type Vector3)
    /// - quelques pérateurs sur les Vector3 peuvent être utiles. Exemple : u=b-a, p=a+k*u (avec a,b,p et u de type Vector3)
    /// - u1.dot(u2) : produit scalaire

    // c = (un pt du plan qconque)
    // v = droite p1p2
    // k = (c*normal - p1*normal) / v*normal
    // I = point d'intersection du plan avec la droite p1p2
    // I = p1+k*p1p2
    Vector3 p1p2(p1,p2);
    double k = (_tabVertex[0]->point().dot(_normal) - p1.dot(_normal)) / p1p2.dot(_normal);
    Vector3 kp1p2 = k * p1p2;
    Vector3 res = p1 + kp1p2;

    return res;
}


/// coupe la facette this selon f (i.e. this est découpée par le plan porteur de f1) :
/// précondition : this doit être **convexe**
void FaceBSP::separate(const FaceBSP &f) {
    /// il faut séparer les sommets de this dans les tableaux vertexNegative et vertexPositive (algo de séparation vu en cours)
    /// _tabVertex[i] vous donne le i-ième sommet de la face this.
    /// _tabVertex.size() vous donne le nombre de sommet de this.
    /// vertexNegative.push_back(un_VertexBSP *) ajoutera le sommet à vertexNegative.
    /// lors d'une intersection : il faut créer un nouveau sommet : utilisez un_vertex=createVertex(un_vector3) (avec le résultat un_vertex de type VertexBSP *)
    ///                           NE PAS faire de inter=new VertexBSP() (gestion mémoire assurée par createVertex)

    vector<VertexBSP *> vertexNegative;
    vector<VertexBSP *> vertexPositive;
    vertexNegative.clear();
    vertexPositive.clear();

    // TODO : à compléter




    // A LAISSER ! (construction des faces selon un tableau)
    createFace(vertexNegative,vertexPositive);
    if ((_facePositive==NULL) && (_faceNegative==NULL)) throw Error("Face découpée en rien du tout (ni plus, ni moins)",__LINE__,__FILE__);

}

/** ************************************************************************ */
/** ************************************************************************ */

void FaceBSP::createFace(const vector<VertexBSP *> &moins,const vector<VertexBSP *> &plus) {
    if (moins.empty()) {
        _faceNegative=NULL;
        _facePositive=this;
    }
    else if (plus.empty()) {
        _facePositive=NULL;
        _faceNegative=this;
    }
    else {
        this->copyVertexFrom(plus);
        _facePositive=this;
        _faceNegative=createFace(moins);
    }
}

void FaceBSP::copyVertexFrom(const vector<VertexBSP *> &liste ) {
    _tabVertex.clear();
    for(unsigned int i=0;i<liste.size();i++) {
        _tabVertex.push_back(liste[i]);
    }
}

FaceBSP *FaceBSP::createFace(const vector<VertexBSP *> &liste) {
    if (liste.empty())
        return NULL;
    else {
        FaceBSP *res;
        res=_owner->createFace();
        for(unsigned int i=0; i<liste.size(); i++) {
            res->add(liste[i]);
        }
        res->normal(this->normal());
        return res;
    }
}


FaceBSP::FaceBSP(ObjetBSP *owner) {
    _owner=owner;
    _owner->add(this);
    _tabVertex.clear();
    _markDelete=false;

    _faceNegative=NULL;
    _facePositive=NULL;
}

VertexBSP *FaceBSP::createVertex(const Vector3 &p) {
    VertexBSP *res=_owner->createVertex(p);
    return res;
}


void FaceBSP::draw() {
    glBegin(GL_POLYGON);
    for(unsigned int i=0; i<_tabVertex.size(); i++) {
        glNormal3dv(_tabVertex[i]->normal().dv());
        glVertex3dv(_tabVertex[i]->point().dv());
    }
    glEnd();
}

void FaceBSP::add(VertexBSP *v) {
    //  if (_tabVertex.empty() || (((_tabVertex[0]->point().distance2(v->point())>0.0000001) && ((_tabVertex[_tabVertex.size()-1]->point().distance2(v->point())>0.000001)))))
    _tabVertex.push_back(v);
}


