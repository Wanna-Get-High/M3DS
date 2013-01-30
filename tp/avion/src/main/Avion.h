#ifndef INC_OBJET_H
#define INC_OBJET_H

#include "Quaternion.h"
#include "Vector3.h"
#include "MeshObj.h"
#include <string>


class Avion : public prog3d::MeshObj {
private:
  double _angleX,_angleY,_angleZ;
  double _increment,_accelerate,_vitesse;

  prog3d::Vector3 _position;
  prog3d::Quaternion _orientation;

public:
  Avion();
  virtual ~Avion();
  void drawWorld();
  void drawLocal();
  void read(const std::string &filename);

  inline const prog3d::Vector3 &position() const {return _position;}
  inline const prog3d::Quaternion &orientation() const {return _orientation;}

  void move();
  void pitchUp();
  void pitchDown();
  void rollLeft();
  void rollRight();
  void yawLeft();
  void yawRight();
  void accelerate();
  void decelerate();
};

#endif
