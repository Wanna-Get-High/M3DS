#include "Avion.h"
#include "UtilGL.h"

using namespace prog3d;
using namespace std;

Avion::Avion() {
  _increment=1.0;
  _accelerate=0.01;
  _vitesse=0.0;
  _orientation.setIdentity();
  _position.set(0,0,0);
}

Avion::~Avion() {

}


/* Tracer l'objet :
Utilisez les attributs suivants :
  - _vertex : tableau de type Vector3 contenant les coordonnées des sommets (exemple : _vertex[3].x() = coordonnée x du sommet d'indice 3)
  - _face : un tableau d'entiers à deux dimensions (un tableau de tableau) contient les indices des sommets de chaque polygone :
      Exemple : _face[3] décrit la face 3; _face[3][4] donne l'indice (dans le tableau _vertex) du 5ième sommet de la face 3.
  - _face.size() et _vertex.size() donnent le nombre de faces et de sommets de l'objet.
  - _face[i].size() donne le nombre de sommets de la face i
*/
void Avion::drawLocal() {
  glColor3f(0,0.2,0.5);

}

void Avion::drawWorld() {
  glPushMatrix();

  drawLocal();

  glPopMatrix();
}

void Avion::move() {
}

void Avion::pitchUp() {
  _angleX+=_increment;
}

void Avion::pitchDown() {
  _angleX-=_increment;
}

void Avion::rollLeft() {
  _angleZ+=_increment;
}

void Avion::rollRight() {
  _angleZ-=_increment;
}

void Avion::yawLeft() {
  _angleY+=_increment;
}

void Avion::yawRight() {
  _angleY-=_increment;
}

void Avion::accelerate() {
  _vitesse+=_accelerate;
}

void Avion::decelerate() {
  _vitesse-=_accelerate;
}

// lecture fichier .obj
void Avion::read(const string &filename) {
  this->MeshObj::read(filename);
  this->scaleInBoxMin(-1,1,-1,1,-1,1);
  this->computeNormal();
}


