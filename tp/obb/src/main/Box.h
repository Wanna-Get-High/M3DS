#ifndef BOITE_H
#define BOITE_H

/**
@file
@author F. Aubert
@brief opérations sur OBB pour mini simu physique (collision+impulsion)

*/


#include "Vector3.h"
#include "CollisionInfo.h"
#include "Texture.h"
#include "Color.h"

class CollisionInfo;

class Box
{
  public:
    Box();

    void position(const prog3d::Vector3 &p) {_position=p;}
    void velocity(const prog3d::Vector3 &v) {_velocity=v;}

    void theta(double a) {_theta=a;}
    void omega(const prog3d::Vector3 &w) {_omega=w;}

    void mass(double m) {_mass=m;}
    void dimension(double w,double h);
    void computeInertia();
    prog3d::Vector3 computeMoment(const prog3d::Vector3 &vect,const prog3d::Vector3 &application);

    void texture(prog3d::Texture *tex) {_texture=tex;}

    const prog3d::Vector3 &position() const {return _position;}
    const prog3d::Vector3 &velocity() const {return _velocity;}
    double mass() const {return _mass;}
    double width() const {return _width;}
    double height() const {return _height;}
    double theta() const {return _theta;}
    const prog3d::Vector3 &omega() const {return _omega;}
    double inertia() {return _inertia;}

    void enableVisualEffect(unsigned int i);
    void disableVisualEffect(unsigned int i);

    void addForce(const prog3d::Vector3 &f);
    void addMoment(const prog3d::Vector3 &f,const prog3d::Vector3 &application);
    void addMoment(const prog3d::Vector3 &m);

    const prog3d::Vector3 &force() const {return _cumulForce;}
    const prog3d::Vector3 &moment() const {return _cumulMoment;}

    void resetForce() {_cumulForce.set(0,0,0);}
    void resetMoment() {_cumulMoment.set(0,0,0);}

    void addVelocityCorrec(const prog3d::Vector3 &v);
    void addPositionCorrec(const prog3d::Vector3 &p);
    void addOmegaCorrec(const prog3d::Vector3 &w);

    void velocityCorrection();
    void positionCorrection();
    void omegaCorrection();


    prog3d::Vector3 vertex(unsigned int i) const;

    prog3d::Vector3 toWorld(const prog3d::Vector3 &p) const;
    prog3d::Vector3 toLocal(const prog3d::Vector3 &p) const;

    void draw();

    void attachWorld(const prog3d::Vector3 &p);
    const prog3d::Vector3 &attachLocal();

    prog3d::Vector3 attachWorld();


    static bool detectCollision(Box *b1, Box *b2, CollisionInfo *collision);
    static void distance(Box *b1,Box *b2,const prog3d::Vector3 &u,double *distance,double *sign);
    static void drawDebugProject(const Box *b1,const Box *b2,const prog3d::Vector3 &axe,double d1,double f1,double d2,double f2);

    void project(const prog3d::Vector3 &axis,double *kmin,double *kmax) const;

    inline void color(const prog3d::Color &c) {_color.set(c);}
    inline const prog3d::Color& color() const {return _color;}

    // axes x et y de la boite
    prog3d::Vector3 directionX() const;
    prog3d::Vector3 directionY() const;

    bool isInside(const prog3d::Vector3 &p) const;

    virtual ~Box();
  protected:
  private:

  // placement boite
  prog3d::Vector3 _position;
  double _theta;
  double _width,_height;

  // propriétés physiques
  double _mass;
  double _inertia;
  prog3d::Vector3 _velocity;


  // simulation
  prog3d::Vector3 _omega;
  prog3d::Vector3 _cumulForce;
  prog3d::Vector3 _cumulMoment;

  prog3d::Vector3 _vCorrec;
  prog3d::Vector3 _pCorrec;
  prog3d::Vector3 _wCorrec;

  prog3d::Color _color;
  prog3d::Texture *_texture;


  unsigned int _visualEffect;

  // application force
  prog3d::Vector3 _attach;

};

#endif // BOITE_H

