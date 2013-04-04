#include "Particle.h"
#include "Quaternion.h"
#include "glsupport.h"
#include "UtilGL.h"

/**
@file
@author F. Aubert
@brief Particle

*/

#include <ctime>
#include <cstdlib>


using namespace std;
using namespace prog3d;




/** random number in [a,b] **/
double randDouble(double a, double b) {
  double c=double(rand())/RAND_MAX;
  return c*(b-a)+a;
}

double randUnit() {
  return double(rand())/RAND_MAX;
}


/** Données initiales d'une particule
**/
void Particle::initRandom() {

  this->position(Vector3(randDouble(-1,1),5,0));

  //this->velocity(2.0*Vector3(randDouble(-1,1),randDouble(0,5),randDouble(-1,1)));
  _timeOfLife=5; // en secondes

  _color.set(randDouble(0,1),randDouble(0,1),0.8);


  _mass=1.0;

  this->birth(); // se contente de conserver l'instant de la naissance (pour gérér la durée de vie)
}

/** *********************************************************** **/


Particle::Particle() {
  //ctor
  _mass=1;
  _velocity.set(0,0,0);
  _life=false;
  _radius=0.5;

  _pCorrec=Vector3(0,0,0);
  _vCorrec=Vector3(0,0,0);
}

Particle::~Particle() {
  //dtor
}


const Vector3 &Particle::position() const {
  return _position;
}

const Vector3 &Particle::velocity() const {
  return _velocity;
}


void Particle::position(const Vector3 &p) {
  _position=p;
}

void Particle::velocity(const Vector3 &v) {
  _velocity=v;
}

void Particle::kill() {
  _life=false;
}

void Particle::birth() {
  _birth=double(clock())/CLOCKS_PER_SEC;
  _life=true;
}

void Particle::texture(Texture *t) {
  _texture=t;
}

double Particle::mass() {
  return _mass;
}

void Particle::mass(double m) {
  _mass=m;
}


void Particle::drawSphere() {
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glPushMatrix();
  glTranslatef(_position.x(),_position.y(),_position.z());

  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Vector4(_color.x(),_color.y(),_color.z(),0.0).fv());
  glScalef(_radius,_radius,_radius);
  UtilGL::drawSphere();
  glPopMatrix();
  glPopAttrib();
}


// TODO : un peu trop bourrin.
void Particle::drawParticle() {
  glEnable(GL_TEXTURE_2D);
  _texture->enable();

  float m[16];
  glPushMatrix();
  glTranslatef(_position.x(),_position.y(),_position.z());
  glGetFloatv(GL_MODELVIEW_MATRIX,m);
  Matrix4 mat;
  Vector3 translation(m[12],m[13],m[14]);

  mat.setTranslation(translation);

  glPushMatrix();
  glLoadIdentity();
  glMultMatrixd(mat.dv());

  glScalef(_radius,_radius,_radius);


  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  _texture->enable();
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glColor4f(_color.x(),_color.y(),_color.z(),1.0);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);

  glScalef(_radius,_radius,_radius);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(-1,-1,0);
  glTexCoord2f(1,0);
  glVertex3f(1,-1,0);
  glTexCoord2f(1,1);
  glVertex3f(1,1,0);
  glTexCoord2f(0,1);
  glVertex3f(-1,1,0);
  glEnd();
  glPopMatrix();

  glPopAttrib();

  glEnable(GL_LIGHTING);

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


}

void Particle::updateLife() {
    double elapsed=double(clock())/CLOCKS_PER_SEC-_birth;
    if (elapsed>_timeOfLife) {
      kill();
    }
}

bool Particle::dead() {
  return !_life;
}

bool Particle::alive() {
  return _life;
}

void Particle::addForce(const Vector3 &f) {
  _cumulForce+=f;
}

const Vector3 &Particle::force() {
  return _cumulForce;
}

void Particle::resetForce() {
  _cumulForce.set(0,0,0);
}

void Particle::timeOfLife(double t) {
  _timeOfLife=t;
}

double Particle::timeOfLife() {
  return _timeOfLife;
}

double Particle::radius() {
  return _radius;
}

void Particle::radius(double r) {
  _radius=r;
}

void Particle::addVelocityCorrec(const Vector3 &v) {
  _vCorrec+=v;
}

void Particle::addPositionCorrec(const Vector3 &p) {
  _pCorrec+=p;
}



void Particle::velocityCorrection() {
  _velocity+=_vCorrec;
  _vCorrec=Vector3(0,0,0);
}

void Particle::positionCorrection() {
  _position+=_pCorrec;
  _pCorrec=Vector3(0,0,0);
}



