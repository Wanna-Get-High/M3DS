#include <math.h>
#include "Voiture.h"
#include "glsupport.h"
#include "UtilGL.h"
#include "Quaternion.h"

using namespace prog3d;
using namespace std;



// ******************************************************************
// méthodes à compléter lors du TP
void Voiture::tracerJante() {
}

void Voiture::tracerRoue() {
}

void Voiture::tracerEssieu() {
}

void Voiture::tracerCarrosserie() {
}

void Voiture::drawLocal() {
}


void Voiture::drawWorld() {
  Vector3 u;
  double a;
  _orientation.copyToAngleAxis(&a,&u); // permet de covertir le quaternion en rotation d'angle a et d'axe u, qu'on pourra alors donner à OpenGL

  glPushMatrix();
  drawLocal();
  glPopMatrix();
}

void Voiture::avancer() {
}

void Voiture::reculer() {
}


// ************************************************************
Voiture::Voiture() {
  _orientation.setIdentity();
  _position.set(0,-1.0,0);
}


