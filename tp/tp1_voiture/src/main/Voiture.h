#ifndef INC_VOITURE_H
#define INC_VOITURE_H

/**
  @author F. Aubert
  **/

#include "Vector3.h"
#include "Quaternion.h"

class Voiture {
  prog3d::Vector3 _position;
  prog3d::Quaternion _orientation;

  float _angle; // rotation roue
  float _braquage; // rotation/essieu

public:
  Voiture();
  void drawWorld();
  void drawLocal();

  void tracerJante();
  void tracerRoue();
  void tracerEssieu();
  void tracerCarrosserie();

  void angle(double a) {_angle=a;}
  void braquage(double b) {_braquage=b;}

  void avancer();
  void reculer();
};

#endif
