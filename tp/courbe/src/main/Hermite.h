#ifndef HERMITE_H
#define HERMITE_H

#include "Vector3.h"

/**
@author F. Aubert
*/

class Hermite
{
  public:
    Hermite();
    virtual ~Hermite();
    Hermite(const prog3d::Vector3 &a,const prog3d::Vector3 &na,const prog3d::Vector3 &b,const prog3d::Vector3 &nb);
    void set(const prog3d::Vector3 &a,const prog3d::Vector3 &na,const prog3d::Vector3 &b,const prog3d::Vector3 &nb);
    prog3d::Vector3 eval(double t);
    void draw();


  protected:
  private:
    prog3d::Vector3 _a;
    prog3d::Vector3 _b;
    prog3d::Vector3 _na;
    prog3d::Vector3 _nb;
};

#endif // HERMITE_H
