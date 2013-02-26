#include "CatmullRomCurve.h"
#include "Vector3.h"
#include "Error.h"
#include "UtilGL.h"

/**
@file
@author F. Aubert
*/


using namespace prog3d;
using namespace std;


/**
* Tracé de la courbe d'interpolation
*/
void CatmullRomCurve::draw() {

    // A COMPLETER :
    // on doit tracer nbPoint()-1 courbes de bézier cubiques : la ième courbe a pour points de controle point(i),
    // intermediate(i,0),intermediate(i,1) et point(i+1)
    // il suffit donc d'affecter la variable "cubic" avec ces points (par cubic.point(j,<un Vector3>)), puis de la tracer

    // - intermediate(i,0) et intermediate(i,1) vous donnent les deux points intermédiaires pour la courbe i
    // (définie entre point(i) et point(i+1))
    // - UtilGL::draw(p1,p2) permet de tracer un segment entre p1 et p2 (de type Vector3)

    Curve cubic;
    cubic.create(4);
    Vector3 p1,p2;
    int size  = nbPoint();

    if (size<2) return; // on déclenche le calcul uniquement si plus de 2 points saisis.

    for (int i = 0; i<size-1; i++) {
        cubic.point(0,point(i));
        cubic.point(1,intermediate(i,0));
        cubic.point(2,intermediate(i,1));
        cubic.point(3,point(i+1));

        cubic.drawBezier();
    }

}

/**
*  Calcul des points intermédiaires pour obtenir une continuité G1 par Catmull-Rom
*/
void CatmullRomCurve::setup() {
    Vector3 parallele;

    // A COMPLETER : il s'agit d'initialiser les points intermédiaires (les points verts)
    // en appelant les setters intermediate(i,0,<un Vector3>) et intermediate(i,1,<un Vector3>) )
    // les points intermédiaires doivent être fixés par la méthode vue en cours
    // (parallèle au segment [point(i-1),point(i+1)]).

    int size = nbPoint();
    for (int i = 1; i < size-1; i++) {
        parallele = Vector3(point(i-1),point(i+1));
        intermediate(i-1,1,point(i)-(parallele/2));
        intermediate(i,0,point(i)+(parallele/2));
    }
}


/** *********************************************************************************************************** */

CatmullRomCurve::CatmullRomCurve() {
    _intermediate.clear();
}

CatmullRomCurve::~CatmullRomCurve() {
}

const Vector3 &CatmullRomCurve::intermediate(unsigned int i,unsigned int j) const {
    return _intermediate[2*i+j];
}

void CatmullRomCurve::intermediate(unsigned int i,unsigned int j,const Vector3 &p) {
    _intermediate[2*i+j].set(p);
}

void CatmullRomCurve::addIntermediate() {
    int i=nbPoint()-2;
    if (i>=0) {
        if (_intermediate.size()!=(nbPoint()-2)*2) throw Error("incoherence between pts and intermediate",__LINE__,__FILE__);
        Vector3 p1=point(i)+0.33*(point(i+1)-point(i));
        Vector3 p2=point(i)+0.66*(point(i+1)-point(i));
        _intermediate.push_back(p1);
        _intermediate.push_back(p2);
    }
}


void CatmullRomCurve::drawPoint() {
    for(unsigned int i=0; i<nbPoint(); i++) {
        UtilGL::draw(point(i));
        UtilGL::draw(i,point(i)+Vector3(0.02,0.02,0.0));
    }
}

void CatmullRomCurve::addEnd(const Vector3 &p) {
    Curve::addEnd(p);
    addIntermediate();
}

vector<Vector3> *CatmullRomCurve::intermediatev() {
    return &_intermediate;
}

void CatmullRomCurve::drawControl(int mode) {
    if (nbPoint()>0) {
        if (mode>0) {
            glColor3f(0,0,1);
            if (mode==1) {
                drawPoint();
            }
            if (mode>1) {
                Curve::drawControl();
                if (mode>2) {
                    glColor3f(0,1,0);
                    for(unsigned int i=0; i<nbPoint()-1; i++) {
                        UtilGL::draw(point(i),intermediate(i,0));
                        UtilGL::draw(intermediate(i,0),intermediate(i,1));
                        UtilGL::draw(intermediate(i,1),point(i+1));
                        UtilGL::draw(intermediate(i,0));
                        UtilGL::draw(intermediate(i,1));
                    }
                }
            }
        }
    }
}



