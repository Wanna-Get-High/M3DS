#ifndef ENGINE_H
#define ENGINE_H

#include "ParticleList.h"
#include "Plane.h"
#include <ctime>
/**
@file
@author F. Aubert
@brief Basic Particle Engine

*/


class Engine
{
  public:
    Engine();
    virtual ~Engine();

    void addPlane(prog3d::Plane *p);

    void particleList(ParticleList *p);
//    void boxList(BoxList *b);

    void computeForce();
    void collisionPlane();
    void euler(double dt);

    void draw();
    void modeParticle(bool mode);

    void run();
    void update();

    void updatePositionVelocity();

    int nbParticle();

    void interCollision();

    double computeImpulse(Particle *p1, Particle *p2,const prog3d::Vector3 &n, double restitution);


    void resetForce();

    void enableWind(const prog3d::Line &ray);
    void disableWind();
protected:
  private:
  ParticleList *_particleList;
//  BoxList *_boxList;
  std::vector<prog3d::Plane *> _planeList;
  clock_t _start;
  float _dt;

  bool _modeParticle;
  prog3d::Line _wind;
  bool _windEnable;
};

#endif // ENGINE_H

