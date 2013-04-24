#include <cmath>
#include "UtilGL.h"
#include <iostream>
#include <string>
#include <sstream>

#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Color.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

using namespace prog3d;
using namespace std;

#define MY_PI 3.141592653

/** Class attributes */
std::vector<Vector3> UtilGL::_debugPoint;
std::vector<Color> UtilGL::_debugPointColor;
std::vector<Vector3> UtilGL::_debugDecal;
std::vector<std::string> UtilGL::_debugMess;

std::vector<Vector3> UtilGL::_debugSegmentA;
std::vector<Vector3> UtilGL::_debugSegmentB;
std::vector<Color> UtilGL::_debugSegmentColor;
std::vector<std::string> UtilGL::_debugSegmentMess;

prog3d::Vector2 UtilGL::_currentTextPos,UtilGL::_initTextPos;


QGLWidget *UtilGL::_widgetQt=NULL;
QFont *UtilGL::_fontQt=NULL;

/** ********************** */

UtilGL::UtilGL() {
    cout << "bad use of UtilGL : use UtilGL::init() and UtilGL::kill() to activate functionalities of UtilGL" << endl;
	exit(1);
}

UtilGL::~UtilGL() {
}

void UtilGL::init(QGLWidget *widget) {
	_widgetQt=widget;
	_fontQt=new QFont("Helvetica [Cronyx]", 36);
	_fontQt->setBold(true);
	_fontQt->setPointSize(12);
}

void UtilGL::kill() {
  if (_fontQt) delete _fontQt;
}

void UtilGL::drawSphere(unsigned int nbSlice,unsigned int nbStack) {
  GLUquadric *q=gluNewQuadric();
  gluSphere(q,1,nbSlice,nbStack);
  gluDeleteQuadric(q);
}



void UtilGL::drawGround(const prog3d::Texture &texture) {
  glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texture.id());
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glPushMatrix();
  glRotatef(-90,1,0,0);
  glTranslatef(0,0,0);
  glScalef(50,50,1);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glScalef(20,20,1);
  glMatrixMode(GL_MODELVIEW);
  drawImmediateSquare();
  glPopMatrix();
  glMatrixMode(GL_TEXTURE);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_TEXTURE_2D);
  glPopAttrib();
}



void UtilGL::drawGrid() {
  glColor3f(0,0,0);

  float pasX;
  float pasY;
  float demiLongueur=10;  // les coordonnées varient entre -demiLongeur et demiLongueur.
  int nbCase=50;       // nbre de cases de la grille en x et y : taille de la grille en cases : nbCase x nbCase
  float posX,posY;

  pasX=demiLongueur*2.0/float(nbCase);
  pasY=demiLongueur*2.0/float(nbCase);

  posX=-demiLongueur;
  for(int i=0; i<nbCase; i++) {
    glBegin(GL_QUAD_STRIP);
    posY=-demiLongueur;
    for(int j=0; j<nbCase+1; j++) {
      glVertex2f(posX,posY);
      glVertex2f(posX+pasX,posY);
      posY+=pasY;
    }
    glEnd();
    posX+=pasX;
  }

}


void UtilGL::draw(const Vector3 &p,int size) {
  glPushAttrib(GL_LIGHTING_BIT | GL_POINT_BIT);
  glDisable(GL_LIGHTING);
  glPointSize(size);
  glBegin(GL_POINTS);
  glVertex3dv(p.dv());
  glEnd();
  glPopAttrib();
}

void UtilGL::draw(const Vector3 &p1,const Vector3 &p2,int size) {
  glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT);
  glDisable(GL_LIGHTING);
  glLineWidth(size);
  glBegin(GL_LINES);
  glVertex3dv(p1.dv());
  glVertex3dv(p2.dv());
  glEnd();
  glLineWidth(1);
  glPopAttrib();
}


void UtilGL::drawCone(double r,double h) {
      double theta=0.0; // angle total
      unsigned int numt=20;
      double past=2*MY_PI/(double)numt;

    double x0,y0;
    double nx0,ny0,nz0;
    nx0=h;ny0=0;nz0=r;
    double nx1,ny1,nz1;

    glBegin(GL_QUAD_STRIP);
    for(unsigned int i=0;i<=numt;i++) {
      x0=cos(theta)*r;
      y0=sin(theta)*r;

      nx1=cos(theta)*nx0-sin(theta)*ny0;
      ny1=sin(theta)*nx0+cos(theta)*ny0;
      nz1=nz0;
      glNormal3f(nx1,ny1,nz1);
      glVertex3f(x0,y0,0);
      glVertex3f(0,0,h);
      theta+=past;

    }
    glEnd();
}

void UtilGL::drawTorus(double rg,double rp) {
  // un peu bourrin, mais tant pis
      double theta=0.0; // angle total
      double phi=0.0;   // angle section
      int numt=20;
      int nump=10;
      double past=2*MY_PI/(double)numt;
      double pasp=2*MY_PI/(double)nump;

      double nx0,ny0,nz0,nx1,ny1,nz1;
      double x0,y0,z0; // rotation du point de section
      double x1,y1,z1; // point de section local section
      for(int i=0;i<numt;i++) {
        phi=0.0;
        glBegin(GL_QUAD_STRIP);
          for(int j=0;j<nump+1;j++) {
            nx1=cos(phi);
            ny1=0.0;
            nz1=sin(phi);
            x1=rp*nx1+(rg-rp);
            y1=0.0;
            z1=rp*nz1;
            for(int k=0;k<2;k++) {
              nx0=(nx1*cos(theta)-ny1*sin(theta));
              ny0=(nx1*sin(theta)+ny1*cos(theta));
              nz0=nz1;

              x0=(x1*cos(theta)-y1*sin(theta));
              y0=(x1*sin(theta)+y1*cos(theta));
              z0=z1;
              glNormal3f(nx0,ny0,nz0);
              glVertex3f(x0,y0,z0);
              theta+=past;
            }
            theta-=2.0*past;
            phi+=pasp;
          }
        glEnd();
        theta+=past;
      }
}


void UtilGL::drawImmediateSquare() {
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);
  glVertex2f(-1,-1);
  glTexCoord2f(1,0);
  glVertex2f(1,-1);
  glTexCoord2f(1,1);
  glVertex2f(1,1);
  glTexCoord2f(1,0);
  glVertex2f(-1,1);
  glEnd();
}

void UtilGL::drawImmediateCube() {
  glPushMatrix();
  glTranslatef(0,0,1);
  drawImmediateSquare();
  glPopMatrix();

  glPushMatrix();
  glRotatef(180,0,1,0);
  glTranslatef(0,0,1);
  drawImmediateSquare();
  glPopMatrix();

  glPushMatrix();
  glRotatef(90,0,1,0);
  glTranslatef(0,0,1);
  drawImmediateSquare();
  glPopMatrix();

  glPushMatrix();
  glRotatef(-90,0,1,0);
  glTranslatef(0,0,1);
  drawImmediateSquare();
  glPopMatrix();

  glPushMatrix();
  glRotatef(90,1,0,0);
  glTranslatef(0,0,1);
  drawImmediateSquare();
  glPopMatrix();

  glPushMatrix();
  glRotatef(-90,1,0,0);
  glTranslatef(0,0,1);
  drawImmediateSquare();
  glPopMatrix();

}


void UtilGL::drawCube(float size) {
  glPushMatrix();
  glScaled(size/2.0,size/2.0,size/2.0);
  drawImmediateCube();
  glPopMatrix();
}



void UtilGL::drawCylinder() {
  GLUquadric *cylindre=gluNewQuadric();
  gluCylinder(cylindre,1,1,1,30,1);
  gluDeleteQuadric(cylindre);
}


void UtilGL::materialBlueGreen() {
  GLfloat ambient[4]= {0.1,0.1,0.1,1.0};
  GLfloat diffus[4]= {0,0.4,0.7,1.0}; // diffus bleu-vert
//  GLfloat speculaire[4]= {0,0.3,0.0,1.0}; // spéculaire vert
  GLfloat speculaire[4]= {0,0,0.0,0.0}; // pour tp 1 : pas de spéculaire
  GLfloat brillance=50;

  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffus);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,speculaire);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,brillance);
}


void UtilGL::drawArrow(const Vector3 &a,const Vector3 &u,double radius,const string &s1,const string &s2,double arrowRatio) {
  Vector3 to;
  to.set(a+u*(1.0-arrowRatio));

  glPushMatrix();

  glPushMatrix();
  glTranslatef(a.x(),a.y(),a.z());
  if (!s1.empty()) {
    draw(s1,Vector3(0,0,0));
  }
  glPopMatrix();


  glPushMatrix();
  Quaternion q;
  q.set(Vector3(0,0,1),u);
  double angle;Vector3 axe;
  q.copyToAngleAxis(&angle,&axe);
  glTranslatef(a.x(),a.y(),a.z());
  glRotatef(angle,axe.x(),axe.y(),axe.z());
  glScalef(radius,radius,(to-a).length());
  drawCylinder();
  glPopMatrix();

  if (u.length()>0.0001) {
  glPushMatrix();
  glTranslatef((a+u).x(),(a+u).y(),(a+u).z());

  glPushMatrix();
  if (!s2.empty()) {
    draw(s2,Vector3(0.2*arrowRatio*u.length(),0.03,0));
  }
  glPopMatrix();

  glPopMatrix();

  glPushMatrix();
  glTranslatef(to.x(),to.y(),to.z());
  Matrix4 m;
  m.setRotation(Vector3(0,0,1),u);
  glMultMatrixf(m.fv());



  drawCone(radius*(2.0-arrowRatio),u.length()*arrowRatio);




  glPopMatrix();
  }

  glPopMatrix();

}


void UtilGL::materialRed() {
  GLfloat ambient[4]= {0.1,0.1,0.1,1.0};
  GLfloat diffus[4]= {0.8,0.4,0.0,1.0}; // diffus bleu-vert
  GLfloat speculaire[4]= {0.4,0.4,0.4,1.0}; // spéculaire vert
  GLfloat brillance=50;

  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffus);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,speculaire);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,brillance);
}




void UtilGL::drawAxes(double scale,double radius,double arrowRatio) {
  glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glDisable(GL_LIGHTING);
  Vector3 o(0,0,0);
  Vector3 u;
  glPushMatrix();
  u.set(scale,0,0);
  glColor3f(1,0,0);
  drawArrow(o,u,radius,"","X",arrowRatio);
  u.set(0,scale,0);
  glColor3f(0,1,0);
  drawArrow(o,u,radius,"","Y",arrowRatio);
  u.set(0,0,scale);
  glColor3f(0,0,1);
  drawArrow(o,u,radius,"","Z",arrowRatio);
  glPopMatrix();
  glPopAttrib();
}


void UtilGL::drawSegment(const Vector3 &a,const Vector3 &b) {
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glVertex3dv(a.dv());
    glVertex3dv(b.dv());
    glEnd();
    glPopAttrib();
}


void UtilGL::drawText(const string &s) {

  QFontMetrics metric(*_fontQt);
  QString qs;
  qs=QString::fromStdString(s);
  renderText(qs);
  int width=metric.width(qs);
  _currentTextPos.x(_currentTextPos.x()+width);
}

void UtilGL::renderText(const QString &qs) {
  glPushAttrib(GL_LIGHTING_BIT);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glActiveTexture(GL_TEXTURE0);
  glClientActiveTexture(GL_TEXTURE0);
  glDisable(GL_LIGHTING);
  glColor3f(1,0,0);
  _widgetQt->renderText(_currentTextPos.x(),_currentTextPos.y(),qs,*_fontQt);

  glPopAttrib();
}


void UtilGL::drawText(const string &s,const Vector2 &pos) {
  QFontMetrics metric(*_fontQt);
  QString qs;
  qs=QString::fromStdString(s);
  int heightT=metric.height();

  _currentTextPos=Vector2(pos.x()*_widgetQt->width(),pos.y()*_widgetQt->height()+heightT);
  _initTextPos=_currentTextPos;

  renderText(qs);
  int width=metric.width(qs);
  _currentTextPos.x(_currentTextPos.x()+width);
}

void UtilGL::drawTextLine(const string &s) {
  QFontMetrics metric(*_fontQt);
  QString qs;
  qs=QString::fromStdString(s);
  renderText(qs);
  int width=metric.height();
  _currentTextPos.x(_initTextPos.x());
  _currentTextPos.y(_currentTextPos.y()+width);
}


void UtilGL::initLight(GLint light) {
  GLfloat ambient[4]= {1.0,1.0,1.0,1.0};
  GLfloat diffuse[4]= {1.0,1.0,1.0,1.0};
  GLfloat speculaire[4]= {.4,.4,.4,1};

  glEnable(light);
  glLightfv(light,GL_AMBIENT,ambient);
  glLightfv(light,GL_DIFFUSE,diffuse);
  glLightfv(light,GL_SPECULAR,speculaire);
}

void UtilGL::materialFrontBack() {
  GLfloat ambientf[4]= {0.9,0.0,0.0,1.0};
  GLfloat diffusf[4]= {0,0.0,0.0,1.0};
  GLfloat speculairef[4]= {0,0.0,0.0,1.0};
  GLfloat brillance=50;

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientf);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffusf);
  glMaterialfv(GL_FRONT,GL_SPECULAR,speculairef);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,brillance);

  GLfloat ambientb[4]= {0.0,0.9,0,1.0};
  GLfloat diffusb[4]= {0.0,0.0,0,1.0};
  GLfloat speculaireb[4]= {0,0.0,0.0,1.0};

  glMaterialfv(GL_BACK,GL_AMBIENT,ambientb);
  glMaterialfv(GL_BACK,GL_DIFFUSE,diffusb);
  glMaterialfv(GL_BACK,GL_SPECULAR,speculaireb);
}

void UtilGL::draw(const vector<Vector3> &l1,const vector<Vector3> &l2) {
  glBegin(GL_QUAD_STRIP);
  for(unsigned int i=0;i<l1.size();i++) {
    glVertex3dv(l1[i].dv());
    glVertex3dv(l2[i].dv());
  }
  glEnd();
}

void UtilGL::draw(unsigned int i,const Vector3 &u) {
  ostringstream oss;
  oss << i;
  draw(oss.str(),u);

}

void UtilGL::draw(const std::string &s,const Vector3 &u) {
	glPushMatrix();
	GLint view[4];
	glGetIntegerv(GL_VIEWPORT,view);
	_fontQt->setPointSizeF(double(view[2])/50.0);
	_widgetQt->renderText(u.x(),u.y(),u.z(),QString::fromStdString(s),*_fontQt);
	glPopMatrix();
}

void UtilGL::waiting(double delay) {
  double start=double(clock())/CLOCKS_PER_SEC;
  double current;
  do {
    current=double(clock())/CLOCKS_PER_SEC;
  } while (current-start<delay);
}



void UtilGL::drawDebug(bool testdepth) {
  glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT);
  glDisable(GL_LIGHTING);
  if (!testdepth) glDepthFunc(GL_ALWAYS);
  glPointSize(8);
  glBegin(GL_POINTS);
  for(unsigned int i=0; i<_debugPoint.size(); i++) {
    glColor3dv(_debugPointColor[i].dv());
    glVertex3dv(_debugPoint[i].dv());
  }
  glEnd();
  glPointSize(1);
  for(unsigned int i=0; i<_debugPoint.size(); i++) {
    glColor3dv(_debugPointColor[i].dv());
    draw(_debugMess[i],_debugPoint[i]+_debugDecal[i]);
  }

  glLineWidth(5);
  glBegin(GL_LINES);
  for(unsigned int i=0; i<_debugSegmentA.size(); i++) {
    glColor3dv(_debugSegmentColor[i].dv());
    glVertex3dv(_debugSegmentA[i].dv());
    glVertex3dv(_debugSegmentB[i].dv());
  }
  glEnd();



  glPopAttrib();
  if (!testdepth) glDepthFunc(GL_LESS);
}



void UtilGL::addDebug(const Vector3 &p,const string &s,const Color &color,const Vector3 &decal) {
  _debugPoint.push_back(p);
  _debugPointColor.push_back(color);
  _debugDecal.push_back(decal);
  _debugMess.push_back(s);
}


void UtilGL::addDebug(const Vector3 &a,const Vector3 &b,const string &s,const Color &color) {
  _debugSegmentA.push_back(a);
  _debugSegmentB.push_back(b);
  _debugSegmentColor.push_back(color);
  _debugSegmentMess.push_back(s);
}

void UtilGL::debugClear() {
  _debugPoint.clear();
  _debugMess.clear();
  _debugDecal.clear();
  _debugSegmentA.clear();
  _debugSegmentB.clear();
  _debugSegmentMess.clear();


  _debugPointColor.clear();
  _debugSegmentColor.clear();
}


void UtilGL::errorQuery(const string &mesg,int line,const std::string &file) {
  cout << "==============================" << endl;
  GLuint err=glGetError();
  if (err>0) {
    cout << "GL error : " << gluErrorString(err) << " from " << file << " at line " << line << " ("+mesg+")" << endl;
  } else {
    cout << "ok, no GL error : " << " from " << file << " at line " << line << " ("+mesg+")" << endl;
    cout << "==============================" << endl;
  }
}


void UtilGL::material(const prog3d::Vector4 &ambient,const prog3d::Vector4 &diffuse,const prog3d::Vector4 &specular,int shininess) {
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient.fv());
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse.fv());
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular.fv());
  glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
}



void UtilGL::drawBox(const prog3d::Vector3 &a,const prog3d::Vector3 &b,const prog3d::Vector3 &c) {

  glBegin(GL_TRIANGLE_STRIP);
  glVertex3dv((a+b+c).dv());
  glVertex3dv((a+c).dv());
  glVertex3dv((a+b).dv());
  glVertex3dv((a).dv());
  glEnd();


  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(0,0,0);
  glVertex3dv((c).dv());
  glVertex3dv((b).dv());
  glVertex3dv((b+c).dv());
  glEnd();



  glBegin(GL_TRIANGLE_STRIP);
  glVertex3dv((a).dv());
  glVertex3f(0,0,0);
  glVertex3dv((a+b).dv());
  glVertex3dv((b).dv());
  glVertex3dv((a+b+c).dv());
  glVertex3dv((b+c).dv());
  glVertex3dv((a+c).dv());
  glVertex3dv((c).dv());
  glVertex3dv((a).dv());
  glVertex3f(0,0,0);

  glEnd();
}


