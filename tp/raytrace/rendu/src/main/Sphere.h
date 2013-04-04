#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

class IntersectionArray;

#include "Primitive.h"

class Sphere:public Primitive {
    public:
    Sphere();
    virtual prog3d::Vector3 computeNormal(const prog3d::Vector3 &p);
    virtual void drawGL();
    void intersection(const Ray &ray, IntersectionArray *result);
};

#endif // SPHERE_H_INCLUDED

