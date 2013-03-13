#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "Vector3.h"

/*!
*
* @file
*
* @brief Colours (r,g,b,a)
* @author F. Aubert
*
*/



namespace prog3d {
/**
@class Color
@brief Colours (r,g,b,a)
*/

class Color {
    double _c[4];
    public:
    Color();
    Color(const Vector3 &t,double a=1.0);
    Color(double r,double g,double b,double a=1.0);
    virtual ~Color();

    inline void set(double r,double g,double b,double a=1.0) {_c[0]=r;_c[1]=g;_c[2]=b;_c[3]=a;}
    inline void set(unsigned int i,double c) {_c[i]=c;}
    inline void set(const Color &c) {_c[0]=c.r();_c[1]=c.g();_c[2]=c.b();_c[3]=c.a();}

    /// affecter la couleur avec le tableau c (valeurs supposées entre 0 et 255)
    void setBytev(unsigned char *c);

    unsigned char getByteR() const;
    unsigned char getByteG() const;
    unsigned char getByteB() const;

    inline void r(double r) {_c[0]=r;}
    inline void g(double g) {_c[1]=g;}
    inline void b(double b) {_c[2]=b;}
    inline void a(double a) {_c[3]=a;}

    inline double r() const {return _c[0];}
    inline double g() const {return _c[1];}
    inline double b() const {return _c[2];}
    inline double a() const {return _c[3];}

    inline const double *dv() const {return _c;}

    const Color &add(const Color &c);
    const Color &addSature(const Color &c);

    const Color &operator+=(const Color &c);

    friend std::ostream& operator <<(std::ostream &s,const Color &c);
    friend Color prog3d::operator *(double k,const Color &c);
    friend Color prog3d::operator *(const Color &c,double k);
    friend Color prog3d::operator -(const Color &c1,const Color &c2);
    friend Color prog3d::operator +(const Color &c1,const Color &c2);
    friend Color prog3d::operator *(const Color &c1,const Color &c2);

};

}

#endif // COLOR_H_INCLUDED

