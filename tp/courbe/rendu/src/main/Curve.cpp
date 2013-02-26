#include "Curve.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "UtilGL.h"
#include "Error.h"

/**
@file
@author F. Aubert
*/

using namespace prog3d;
using namespace std;

/**
* Evaluation de la Curve de Bézier en $t$
**/
Vector3 Curve::evalBezier(double t) {
    Vector3 result;
    if (nbPoint()>1) {
        vector<Vector3> castel;      // tableau de points sur lesquels on applique deCasteljau
        copyTo(&castel); // on recopie les points de controles dans le tableau castel (castel est donc initialisé avec la première ligne de l'algo triangulaire).

        // A COMPLETER : appliquer la méthode de De Casteljau
        int size = castel.size();

        for (int j = 1; j < size; j++) {
            for (int i = 0; i < size-j; i++) {
                castel[i] = (t*castel[i+1]) + ((1-t)*castel[i]);
            }
       }

        // le point de la courbe doit se trouver dans castel[0] à la fin de l'algo
        result.set(castel[0]);
    }

    return result;
}

/**
  Tracé de la courbe de Bézier
  **/
void Curve::drawBezier() {

    Vector3 oldp,newp;
    float pas=1.0/100.0;
    float t=pas;

    // A COMPLETER : tracer 100 segments en appelant evalBezier
    // (remarque : UtilGL::drawTo(oldp, newp) trace le segment entre oldp et newp)

    if(nbPoint()>1) {
        oldp = _pts[0];

        for(int i=0; i<100; i++){
            t = pas*(1+i);
            newp = evalBezier(t);
            UtilGL::draw(oldp,newp);
            oldp = newp;
        }

    }
}




/** *********************************************************************************************************************** **/


Curve::Curve() {
    _pts.clear();
}

Curve::~Curve() {
}


void Curve::create(int nb) {
    for(int i=0;i<nb;i++) {
        _pts.push_back(Vector3(0,0,0));
    }
}

void Curve::point(int i,const Vector3 &p) {
    _pts[i].set(p);
}

void Curve::insert(const Vector3 &p,unsigned int where) {
    _pts.insert(_pts.begin()+where+1,p);
}


void Curve::drawControl() {
    if (nbPoint()>0) {
        for(unsigned int i=0;i<nbPoint()-1;i++) {
            UtilGL::draw(point(i),point(i+1));
            UtilGL::draw(i,point(i)+Vector3(0.02,0.02,0.0));
            UtilGL::draw(point(i));
        }
        UtilGL::draw(point(nbPoint()-1));
        UtilGL::draw(nbPoint()-1,point(nbPoint()-1)+Vector3(0.02,0.02,0.0));
    }
}

void Curve::drawPoint() {
    for(unsigned int i=0;i<nbPoint()-1;i++) {
        UtilGL::draw(point(i));
        UtilGL::draw(i,point(i)+Vector3(0.02,0.02,0.0));
    }
}

void Curve::addEnd(const Vector3 &p) {
    _pts.push_back(Vector3(p));
}



void Curve::copyTo(vector<Vector3> *tab) {
    tab->clear();
    for(vector<Vector3>::iterator i=_pts.begin();i!=_pts.end();i++) {
        tab->push_back(*i);
    }
}










