#ifndef CURVE_H_INCLUDED
#define CURVE_H_INCLUDED


/*!
*
* @file
*
* @brief Curve operations
* @author F. Aubert
*
*/


#include "Vector3.h"
#include "ControlVector3.h"
#include <vector>

/**
@class Curve
@brief Curve operations (control points are in an array of points)
*/

class Curve {
    std::vector<prog3d::Vector3> _pts;
    public:

    Curve();
    virtual ~Curve();

    void create(int nb);
    void point(int i,const prog3d::Vector3 &p);

    inline const prog3d::Vector3 &point(unsigned int i) {return _pts[i];}
    inline std::vector<prog3d::Vector3> *pointv() {return &_pts;}
    inline unsigned int nbPoint() {return _pts.size();}

    void drawControl();
    void drawPoint();
    void drawBezier();
    prog3d::Vector3 evalBezier(double t);

    inline const prog3d::Vector3 &point(int i) {return _pts[i];}

    void addEnd(const prog3d::Vector3 &p);
    void insert(const prog3d::Vector3 &p,unsigned int where);
    void copyTo(std::vector<prog3d::Vector3> *tab);
};

#endif
