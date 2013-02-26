#include "ArbreBSP.h"
#include <string>

/**
* @author Fabrice Aubert
*/

using namespace std;
using namespace prog3d;



/** ************************************************************** */

ArbreBSP::~ArbreBSP() {
  if (_left) delete _left;
  if (_right) delete _right;
}

ArbreBSP::ArbreBSP() {
  _face=NULL;
  _left=NULL;
  _right=NULL;
}

/// constructeur en affectant chacun des champs
ArbreBSP::ArbreBSP(FaceBSP *f,ArbreBSP *g,ArbreBSP *d) {
  _face=f;
  _left=g;
  _right=d;
}

/// affectation de chacun des champs
void ArbreBSP::set(FaceBSP *f,ArbreBSP *g,ArbreBSP *d) {
  _face=f;
  _left=g;
  _right=d;
}

/// affecte la facette pivot avec f
void ArbreBSP::node(FaceBSP *f) {
  _face=f;
}

/// affecte le noeud gauche
void ArbreBSP::left(ArbreBSP *g) {
  _left=g;
}

/// affecte le noeud droit
void ArbreBSP::right(ArbreBSP *d) {
  _right=d;
}

/// donne la facette du noeud
FaceBSP *ArbreBSP::node() {
  return _face;
}

/// donne le sous arbre gauche
ArbreBSP *ArbreBSP::left() {
  return _left;
}

/// donne le sous arbre droit
ArbreBSP *ArbreBSP::right() {
  return _right;
}

/// nombre de faces
int ArbreBSP::nbFace() {
  return 1+ (right()?right()->nbFace():0) + (left()?left()->nbFace():0);
}


