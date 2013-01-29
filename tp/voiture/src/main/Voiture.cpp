#include <math.h>
#include "Voiture.h"
#include "glsupport.h"
#include "UtilGL.h"
#include "Quaternion.h"

using namespace prog3d;
using namespace std;

#define FACTOR -83

// ******************************************************************
// méthodes à compléter lors du TP
void Voiture::tracerRayon(){
    glPushMatrix();

    glTranslatef(0,-1.5,0);
    glScalef(0.1,3,0.1);
    glRotatef(-90,1,0,0);

    UtilGL::drawCylinder();
    glPopMatrix();
}

void Voiture::tracerJante() {
    glColor3f(1,0,0);
    for (int i = 0 ; i < 4 ; i++){
        glRotatef(45,0,0,1);
        tracerRayon();
    }
}

void Voiture::tracerRoue() {
    glPushMatrix();
    glRotatef(_angle,0,0,1);
    tracerJante();
    glPopMatrix();
    glColor3f(0.2,0.2,0.9);
    float diam = 3;
    float inner_diam = 0.2;
    float rayon = diam/2 + inner_diam;
    UtilGL::drawTorus(rayon,inner_diam);
}

void Voiture::tracerEssieu() {

    int demi_longueur = 3;
    float ratio = pow(2*demi_longueur,-1);

    glPushMatrix();
    glTranslatef(0,0,-demi_longueur);
    glScalef(0.25,0.25,2*demi_longueur);
    glColor3f(1,0,0);
    UtilGL::drawCylinder();
    glScalef(4,4,ratio);
    tracerRoue();
    glTranslatef(0,0,2*demi_longueur);
    tracerRoue();
    glPopMatrix();
}

void Voiture::tracerCarrosserie() {
    glPushMatrix();
    glScalef(3,1,1);
    glColor3f(0,0,0.79);
    UtilGL::drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1,1,0);
    UtilGL::drawCube();
    glPopMatrix();
}

void Voiture::drawLocal() {
    glPushMatrix();
    glScalef(0.5,0.5,0.5);

    glTranslatef(0,-2.8,0);
    glPushMatrix();
    glScalef(3,3,3);
    tracerCarrosserie();
    glPopMatrix();
    glTranslatef(-3,-1.5,0);
    glRotatef(_braquage,0,1,0);
    tracerEssieu();
    glRotatef(-_braquage,0,1,0);
    glTranslatef(6,0,0);
    tracerEssieu();
    glTranslatef(-3,1.5,0);
    glPopMatrix();

}


void Voiture::drawWorld() {
    Vector3 u;
    double a;
    _orientation.copyToAngleAxis(&a,&u); // permet de covertir le quaternion en rotation d'angle a et d'axe u, qu'on pourra alors donner à OpenGL

    glPushMatrix();
    glTranslatef(_position.x(),_position.y(),_position.z());
    glRotatef(a, u.x(), u.y(), u.z());
    drawLocal();
    glPopMatrix();
}

void Voiture::avancer() {
    _angle += _delta_angle;
    _position = _position + _orientation * Vector3(_delta_angle/_factor,0,0);
    _orientation.rotate(_braquage/8,Vector3(0,1,0));

}

void Voiture::reculer() {
    _angle -= _delta_angle;
    _position = _position + _orientation * Vector3(-_delta_angle/_factor,0,0);
    _orientation.rotate(-_braquage/8,Vector3(0,1,0));
}

void Voiture::droite() {
    _braquage = fmax(_braquage-1,MIN_ANGLE);
}

void Voiture::gauche() {
    _braquage = fmin(_braquage+1,MAX_ANGLE);
}




// ************************************************************
Voiture::Voiture() {
    _orientation.setIdentity();
    _position.set(0,0,0);
    _delta_angle = 5;
    _factor = -85;
}
