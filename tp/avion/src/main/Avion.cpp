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
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    for (unsigned i = 0 ; i < _face.size() ; i++){
        glBegin(GL_POLYGON);
        for(unsigned j = 0; j < _face[i].size(); j++){
            Vector3 vertex = _vertex[_face[i][j]];
            glNormal3dv(_normalVertex[_face[i][j]].dv());
            glVertex3f(vertex.x(), vertex.y(), vertex.z());
        }
        glEnd();
    }

}

void Avion::drawWorld() {
    glPushMatrix();

    double angle;
    Vector3 u;
    _orientation.copyToAngleAxis(&angle,&u);

    glTranslatef(_position.x(),_position.y(), _position.z());
    glRotatef(angle, u.x(),u.y(),u.z());

    drawLocal();

    glPopMatrix();
}

void Avion::move() {
    _position = _position + _orientation * Vector3(0,0,_vitesse);
}

void Avion::pitchUp() {
    _orientation.rotate(_increment, Vector3(1,0,0));
}

void Avion::pitchDown() {
    _orientation.rotate(-_increment,Vector3(1,0,0));
}

void Avion::rollLeft() {
    _orientation.rotate(-_increment, Vector3(0,0,1));
}

void Avion::rollRight() {
    _orientation.rotate(_increment, Vector3(0,0,1));
}

void Avion::yawLeft() {
    _orientation.rotate(_increment, Vector3(0,1,0));
}

void Avion::yawRight() {
    _orientation.rotate(-_increment, Vector3(0,1,0));
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


