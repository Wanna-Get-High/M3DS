#ifndef CATMULLROMCURVE_H_INCLUDED
#define CATMULLROMCURVE_H_INCLUDED

#include "Curve.h"
#include "Vector3.h"
#include <iostream>

/**
@author F. Aubert
@brief representation/computation of a catmull rom curve (inherits Curve)
*/

class CatmullRomCurve: public Curve {
    std::vector<prog3d::Vector3> _intermediate; // list of intermediate Bezier control points
                    // between pts[i] and pts[i+1] there should be inter[i*2] and inter[i*2+1] (cubic)

    public:
    CatmullRomCurve();
    virtual ~CatmullRomCurve();

    void addEnd(const prog3d::Vector3 &p);
    std::vector<prog3d::Vector3> *intermediatev(); // get the reference

    void intermediate(unsigned int i,unsigned int j,const prog3d::Vector3 &p);
    const prog3d::Vector3 &intermediate(unsigned int i,unsigned int j) const;

    void setup();
    void draw();
    void drawControl(int mode=0);
    void drawPoint();

private:
    void addIntermediate();

};

#endif
