#include "Hermite.h"

#include <iostream>
#include <sstream>
#include "UtilGL.h"

/**
@file
@author F. Aubert
*/


using namespace prog3d;
using namespace std;

/**
* Evaluation de la courbe de hermite P(t) :
* - _a,_b,_na et _nb sont les données géométriques (points extrèmes et tangentes).
* - on peut utiliser les opérateurs *, + sur les points. Exemple : p=matrix[0+i]*_a+...
**/
Vector3 Hermite::eval(double t) {
  // initialiser la matrice 4x4 : (cf cours)

  Vector3 res;

  // A COMPLETER : calculer le point P(t)

 double m[4][4]={   {2.0,   -2.0,   1.0,    1.0},
                    {-3.0,  3.0,    -2.0,   -1.0},
                    {0.0,   0.0,    1.0,    0.0},
                    {1.0,   0.0,    0.0,    0.0} };

  res = ((t^3*m[0]) +

  return res;
}

/**
* Trace la courbe de hermite (100 points)
**/
void Hermite::draw() {
    Vector3 oldp,newp;
    float pas=1.0/100.0;
    float t=pas;

    // A COMPLETER : tracer 100 segments pour décrire la courbe en appelant eval
    // (remarque : UtilGL::draw(oldp,newp) trace le segment entre oldp et newp)



}


/** **************************************************************************************** */
Hermite::Hermite(const Vector3 &a,const Vector3 &na,const Vector3 &b,const Vector3 &nb) {
  _a=a;
  _b=b;
  _na=na;
  _nb=nb;
}


void Hermite::set(const Vector3 &a,const Vector3 &na,const Vector3 &b,const Vector3 &nb) {
  _a=a;
  _b=b;
  _na=na;
  _nb=nb;
}


Hermite::Hermite() {
  //ctor
}

Hermite::~Hermite() {
  //dtor
}
