#ifndef PARTICULE_H
#define PARTICULE_H

#include "Vector3.h"
#include "Texture.h"
/**
@file
@author F. Aubert
@brief Particle

*/

class Particle {
  public:
    Particle();
    virtual ~Particle();


    const prog3d::Vector3 &position() const;
    const prog3d::Vector3 &velocity() const;

    void position(const prog3d::Vector3 &p);
    void velocity(const prog3d::Vector3 &v);

    void addForce(const prog3d::Vector3 &f);
    void resetForce();

    void addVelocityCorrec(const prog3d::Vector3 &v);
    void addPositionCorrec(const prog3d::Vector3 &p);

    void velocityCorrection();
    void positionCorrection();

    const prog3d::Vector3 &force();

    void mass(double m);
    double mass();

    void texture(prog3d::Texture *t);

    void drawSphere();
    void drawParticle();

    bool dead();
    bool alive();
    void initRandom();
    void birth();
    void updateLife();
    void kill();

    void timeOfLife(double t);
    double timeOfLife();
    double radius();
    void radius(double r);

  protected:
  private:
    prog3d::Vector3 _position;
    prog3d::Vector3 _velocity;

    prog3d::Vector3 _pCorrec;
    prog3d::Vector3 _vCorrec;

    prog3d::Vector3 _color;

    double _mass;

    prog3d::Vector3 _cumulForce;


    bool _life;
    double _timeOfLife;
    double _birth;

    double _radius;

    prog3d::Texture *_texture;
};

#endif // PARTICULE_H

