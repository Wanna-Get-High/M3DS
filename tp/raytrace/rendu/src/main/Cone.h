#ifndef CONE_H_INCLUDED
#define CONE_H_INCLUDED

class IntersectionArray;

#include "Primitive.h"

class Cone:public Primitive {
  typedef enum {FULL_IN, FULL_OUT, INTERSECT_IN,INTERSECT_OUT} ESpaceIntersect;

    public:
    Cone();
    Cone(prog3d::Color a,prog3d::Color d,prog3d::Color s);
    virtual prog3d::Vector3 computeNormal(const prog3d::Vector3 &p);
    virtual void drawGL();
    void intersection(const Ray &ray, IntersectionArray *res);

private:
    Cone::ESpaceIntersect intersectCap(double a,double u,double *res1,double *res2);

};
#endif // CONE_H_INCLUDED

