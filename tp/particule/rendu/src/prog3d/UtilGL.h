#ifndef UTILGL_H_INCLUDED
#define UTILGL_H_INCLUDED

#include <string>
#include <ctime>
#include <cmath>

#include "glsupport.h"

#include "Vector3.h"
#include "Matrix4.h"
#include "Color.h"


#include "Texture.h"


namespace prog3d {
/**
@file
@author F. Aubert

@class UtilGL
@brief drawing utilities (drawCube, drawSphere, etc...)
*/


/**
*/
class UtilGL {
static std::vector<Vector3> _debugPoint;
static std::vector<Color> _debugPointColor;
static std::vector<Vector3> _debugDecal;
static std::vector<std::string> _debugMess;

static std::vector<Vector3> _debugSegmentA;
static std::vector<Vector3> _debugSegmentB;
static std::vector<Color> _debugSegmentColor;
static std::vector<std::string> _debugSegmentMess;


static prog3d::Vector2 _currentTextPos,_initTextPos;


static QGLWidget *_widgetQt;
static QFont *_fontQt;


    /// draws the buffer (have to be initialized)
    static void drawCubeBuffer();


    public:
    UtilGL();
    virtual ~UtilGL();
    /// @brief draw cube
    static void drawCube(float size=1.0);
    /// @brief draw cylinder
    static void drawCylinder();
    /// @brief draw sphere
    static void drawSphereTexture(int nbSlice=10,int nbStack=10);
    static void drawSphere(unsigned int nbSlice=10,unsigned int nbStack=10);
    static void drawCone(double r=1,double h=1);


    static void drawGround(const Texture &texture);


    static void drawGrid();


    static void drawTorus(double rg=1.0,double rp=0.2);
    static void drawImmediateCube();
    static void drawImmediateSquare();

    static void draw(const Vector3 &p,int size=5);
    static void draw(const Vector3 &p1,const Vector3 &p2,int size=2);
    static void draw(const std::vector<Vector3> &l1,const std::vector<Vector3> &l2);
    static void draw(unsigned int i,const Vector3 &u);
    static void draw(const std::string &s,const Vector3 &p);
    static void renderText(const QString &qs);


    static void initCubeBuffer();

    static void waiting(double delay);

    /// draw an arrow of vector u from a : radius = radius of the cylinder, ratio = size of the end in [0,1] (ratio to the total length of u)
    static void drawArrow(const Vector3 &a,const Vector3 &u,double radius=1.0,const std::string &s1="",const std::string &s2="",double arrowRatio=0.2);


    /// affiche le repère local
    static void drawAxes(double scale=1.0,double radius=0.2,double arrowRatio=0.2);


    static void materialBlueGreen();
    static void materialRed();
    static void init(QGLWidget *widget);
    static void kill();

    static void drawSegment(const Vector3 &a,const Vector3 &b);

    static void initLight(GLint light);
    static void materialFrontBack();



    /// trace la liste des points qui ont été sauvegardés par add (à appeler lors du display). Remarque : les points ne sont plus dans la liste après le tracé
    static void drawDebug(bool testdepth=true);
    /// permet de mémoriser le point p qui sera affiché lors du display (permet de débugguer) l'affichage lorsqu'on veut afficher un point hors de la routine display)
    static void addDebug(const Vector3 &p,const std::string &,const Color &color=Color(1,0,0),const Vector3 &decal=Vector3(0.1,0.1,0.0));
    /// permset de mémoriser le segment AB
    static void addDebug(const Vector3 &a,const Vector3 &b,const std::string &mesg="",const Color &color=Color(1,0,0));

    /// enlève tous les points mémorisés dans la liste
    static void debugClear();

    ///
    static void errorQuery(const std::string &mesg,int line,const std::string &s);



    static void drawText(const std::string &s);
    static void drawText(const std::string &s, const prog3d::Vector2 &pos);
    static void drawTextLine(const std::string &s);
    static void drawBox(const prog3d::Vector3 &a, const prog3d::Vector3 &b, const prog3d::Vector3 &c);
};


}
#define errorQueryD(s) errorQuery(s,__LINE__,__FILE__)


#endif // UTILGL_H_INCLUDED

