#include "Segment.h"
#include "Vector3.h"
#include <iostream>

/**


@author F. Aubert

*/
using namespace std;

using namespace prog3d;

void Segment::init() {
		resultatDistance=A;
}

Segment::Segment() {
    init();
    _a.set(0,0,0);
    _b.set(0,0,0);
}

Segment::Segment(const Vector3 &a,const Vector3 &b) {
		init();
		_a.set(a);
		_b.set(b);
	}

Segment::Segment(const Vector2 &a,const Vector2 &b) {
		init();
		_a.set(a.x(),a.y(),0);
		_b.set(b.x(),b.y(),0);
	}


void Segment::print(string mesg) {
    cout << mesg << " : "; _a.print("a=");_b.print("b="); cout << endl;
}

void Segment::setAB(const Vector3 &a,const Vector3 &b) {
		_a.set(a);
		_b.set(b);
}

void Segment::setAB(const Vector2 &a,const Vector2 &b) {
		_a.set(a.x(),a.y(),0);
		_b.set(b.x(),b.y(),0);
}

double Segment::distanceToLine2(const Vector3 &m) {
    Vector3 am(m);
    am.sub(_a);
    Vector3 u(_a,_b);

    double k=am.dot(u)/u.dot(u);

    Vector3 h(u);
    h.mad(k,_a);

    am.set(h);
    am.sub(m);
    return am.length2();

}

double Segment::distance2(const Vector3 &p) {
		Vector3 ab(_b);
		ab.sub(_a);
		Vector3 pab(p);
		pab.sub(_a);
		if (pab.dot(ab)<0) {
			return p.distance2(_a);
		}
		pab.set(p);pab.sub(_b);
		if (pab.dot(ab)>0) {
            return p.distance2(_b);
		}


        return distanceToLine2(p);
	}

double Segment::distance2(const Vector2 &p) {
		return distance2(Vector3(p.x(),p.y(),0));
}

Vector3 Segment::interp(double k) {
    return Vector3((1-k)*_a+k*_b);
}

