#ifndef PRIMITIVE_H_INCLUDED
#define PRIMITIVE_H_INCLUDED


class Primitive;

#include <vector>
#include "Ray.h"
#include "Vector3.h"
#include "Material.h"
#include "IntersectionArray.h"

typedef std::vector<double> ADouble;


class Primitive {
    prog3d::Material _mat;
    public:
    virtual ~Primitive();
    Primitive();

    void material(const prog3d::Material &m);

    virtual void intersection(const Ray &r,IntersectionArray *res)=0;

    const prog3d::Material &material() const;
    void colorGL();

    virtual prog3d::Vector3 computeNormal(const prog3d::Vector3 &p)=0;

    virtual void drawGL()=0;
};


#endif // PRIMITIVE_H_INCLUDED

