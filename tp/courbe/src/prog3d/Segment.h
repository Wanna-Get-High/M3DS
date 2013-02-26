#ifndef SEGMENT_H_INCLUDED
#define SEGMENT_H_INCLUDED

#include "Vector2.h"
#include "Vector3.h"
#include <string>

/**
@file
@author F. Aubert
@brief Operations on segments [a,b]
*/


namespace prog3d {

/**
@class Segment
@brief représentation des segments [a,b]
*/
class Segment {
	Vector3 _a,_b;
	enum Entite {A,B,D};
	Entite resultatDistance;

	void init();
    public:
    /// constructeur par défaut
    Segment();

    inline const Vector3 &a() {return _a;}
    inline const Vector3 &b() {return _b;}

    /// constructeur par Segment(a,b)
	Segment(const Vector3 &a,const Vector3 &b);
    /// constructeur par Segment(a,b) du plan
	Segment(const Vector2 &a,const Vector2 &b);
    /// affiche sur la console (a=...,b=...)
    void print(std::string mesg);
    /// affecte le segment avec [aa,bb]
	void setAB(const Vector3 &a,const Vector3 &b);
    /// affecte le segment dans le plan avec [aa,bb]
	void setAB(const Vector2 &a,const Vector2 &b);
    /// donne le carré de la distance entre le point et p
	double distance2(const Vector3 &p);

    /// donne le carré de la distance entre le point et p (dans le plan)
	double distance2(const Vector2 &p);
    /// donne le Vector3=(1-k)A+kB
	Vector3 interp(double k);

    /// returns the squared distance to the line (ab)
    double distanceToLine2(const Vector3 &m);
};

}

#endif // SEGMENT_H_INCLUDED
