#ifndef INC_VOITURE_H
#define INC_VOITURE_H

/**
  @author F. Aubert
  **/

#include "Vector3.h"
#include "Quaternion.h"
#include "math.h"

#define MAX_ANGLE 10
#define MIN_ANGLE -10

class Voiture {
  prog3d::Vector3 _position;
  prog3d::Quaternion _orientation;

  float _angle; // rotation roue
  float _braquage; // rotation/essieu

private:
  void tracerRayon();
public:
  Voiture();
  void drawWorld();
  void drawLocal();

  void tracerJante();
  void tracerRoue();
  void tracerEssieu();
  void tracerCarrosserie();

  void angle(double a) {_angle=a;}
  void braquage(double b) {_braquage=fmin(fmax(b,-10),10);}

  void avancer();
  void reculer();
  void gauche();
  void droite();
};

#endif
