/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <iostream>

using namespace std;
using namespace prog3d;



/// ctor
GLView::GLView(const QGLFormat &format,QWidget *parent) : QGLWidget(format,parent) {

  setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events
  UtilGL::init(this);

  // Data initializations
  _mouseLeftButton=false; // left button is pressed ?
  _mouseRightButton=false; // right button is pressed ?
  _pressForward=_pressBackward=_pressLeft=_pressRight=false; // arrows key
  _angleUD=_angleLR=0; // angle for animation

  _angle=0;

}


/// dtor
GLView::~GLView() {
  UtilGL::kill();
}

/**
 initialize GL context (with glew on linux) + common set up
 **/
void GLView::initializeGL() {

  // initialization of all available OpenGL functionalities
  if (glewInit()!=GLEW_OK) {
    cout << "glew error : unable to initialize glew" << endl;
    exit(EXIT_FAILURE);
  }

  cout << "GL version :" << glGetString(GL_VERSION) << endl;
  cout << "Shading Language  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

  // common OpenGL set up
  glEnable(GL_DEPTH_TEST);  // hidden parts removal with depth buffer algorithm
  glClearColor(0.9,0.9,0.9,0); // color value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done
  glClearDepth(1.0);       // depth value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done

  // TODO
  // lighting setup
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  UtilGL::materialBlueGreen();
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);

  // drawing polygons set up
  glShadeModel(GL_SMOOTH); // activation remplissage avec interpolation linéaire (Gouraud)
//  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // les polygones seront tracés sans remplissage

  // this exercice begins without lighting :
  glDisable(GL_LIGHTING); // on désactive l'éclairement

  // all drawings will be black (till the next glColor3f instruction)
  glColor3f(0,0,0);


}


/** ***************************************************************************
resize
  **/


/// when the graphics window is sized/resized (including the first time the window is shown)
void GLView::resizeGL(int width, int height) {
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.1,0.1,-0.1,0.1,.3,1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/** ***************************************************************************
  update data
  **/
void GLView::updateData() {
  // modifications des données apportées entre chaque image tracée :

  _angleLR+=0.5;

  updateGL(); // demande, par événement, à Qt de retracer la fenêtre OpenGL
}


/** **********************************************************************
  Events
  **/
void GLView::mousePressEvent(QMouseEvent *event) {
  if (event->button()==Qt::LeftButton) {
    cout << "left mouse button = ";
    _mouseLeftButton=true;

    // TODO
    _trackball.start(event->x(),event->y());

  }
  if (event->button()==Qt::RightButton) {
    cout << "right mouse button = ";
    _mouseRightButton=true;
  }
  cout << event->x() << "," << event->y() << endl;
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  if (_mouseRightButton)
    cout << "move : " << event->x() << "," << event->y() << endl;
  // TODO
  else if (_mouseLeftButton) {
    _trackball.motion(event->x(),event->y());
  }
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
  switch(event->button()){
  case Qt::LeftButton:
    _mouseLeftButton=false;
    // TODO
    _trackball.stop();
    break;
  case Qt::RightButton:
    _mouseRightButton=false;
    break;
  default:break;
  }
}

void GLView::wheelEvent(QWheelEvent *event) {
  cout << "mouse wheel = " << event->delta() << endl;
}


void GLView::keyPressEvent(QKeyEvent *event) {
  if (event->isAutoRepeat())
    QGLWidget::keyPressEvent(event);
  else
    switch(event->key()){
    case Qt::Key_Z:
      _pressForward=true;
      cout << "forward pressed" << endl;
      break;
      // TODO
    case Qt::Key_S:
      _pressBackward=true;
      break;
    case Qt::Key_Q:
      _pressLeft=true;
      break;
    case Qt::Key_D:
      _pressRight=true;
      break;

    default:
      QGLWidget::keyPressEvent(event); // dispatch the event to the parent
    }
}

void GLView::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat())
    QGLWidget::keyReleaseEvent(event);
  else
    switch(event->key()){
    case Qt::Key_Z:
      _pressForward=false;
      cout << "forward released" << endl;
      break;
      // TODO
    case Qt::Key_S:
      _pressBackward=false;
      break;
    case Qt::Key_Q:
      _pressLeft=false;
      break;
    case Qt::Key_D:
      _pressRight=false;
      break;
    default:
      QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
    }
}


/** ******************************************************************* **/
/**
  Drawings
**/

void GLView::drawQuadStripExample() {
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // les polygones seront tracés sans remplissage


  glBegin(GL_QUAD_STRIP);
  glVertex2f(-2.0,-2.0);
  glVertex2f(-2.0,2.0);
  glVertex2f(0.0,-2.0);
  glVertex2f(0.0,2.0);
  glVertex2f(2.0,-1.0);
  glVertex2f(2.0,3.0);
  glEnd();

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // les polygones seront tracés avec remplissage

}

void GLView::drawStrip() {
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // les polygones seront tracés sans remplissage


  float demiLongueur=3.0; // la bande commence à la coordonnée x=-3 et s'achève à la coordonnée x=3
  int nbCase=10; // on veut 10 quadrilatères pour la bande

  float pasX=demiLongueur*2.0/float(nbCase); // "longueur" en x d'une case (i.e. ici 10 cases pour aller de -3 à +3)

  float posX;
  posX=-demiLongueur; // coordonnées x du premier sommet (i.e. début de la bande).

  glBegin(GL_QUAD_STRIP);
  for(int i=0; i<nbCase+1; i++) {
    glVertex2f(posX,-2.0);
    glVertex2f(posX,2.0);

    posX+=pasX;
  }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // les polygones seront tracés avec remplissage
}


void GLView::drawGrid() {

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // les polygones seront tracés sans remplissage

  glColor3f(0,0,0);

  float pasX;
  float pasY;
  float demiLongueur=2;  // les coordonnées varient entre -2 et 2 (dimension de la grille en (x,y) est donc 4 x 4).
  int nbCase=20;       // nbre de cases de la grille en x et y : nbre de cases total qui constitue la grille : nbCase x nbCase
  float posX,posY;

  // a compléter

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // les polygones seront tracés avec remplissage
}

void GLView::drawGround() {
  glPushMatrix();
  glRotatef(-90,1,0,0);
  glTranslatef(0,0,-3);
  glScalef(5,5,1);
  drawGrid();
  glPopMatrix();
}


// tracé d'un carré dans le repère courant
void GLView::drawSquare() {
  // tracé d'un polygone
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // les polygones seront tracés sans remplissage


  glColor3f(0,0,0);
  glBegin(GL_POLYGON);
  glVertex3f(-1,-1,0);
  glVertex3f(1,1,0);
  glVertex3f(1,-1,0);
  glVertex3f(-1,1,0);
  glEnd();


  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // les polygones seront tracés avec remplissage

}

void GLView::drawSecret() {
  float p[]= {-1.0,0.0,-0.5,0.0,-0.3,0.3,1.0,0.3,
              1.0,-0.3,0.9,-0.3,0.9,-0.1,0.6,-0.1,
              0.6,-0.3,-0.5,-0.3,-0.5,-0.1,-0.8,-0.1,
              -0.8,-0.3,-1.0,-0.3
             };

}


void GLView::drawCar() {
  glEnable(GL_LIGHTING); // sera vu plus tard dans le cours
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glPushMatrix();

  glPopMatrix();

  glDisable(GL_LIGHTING);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

}


void GLView::paintGL() {
  // effacer écran (chaque pixel est affecté avec la couleur spécifiée par glClearColor, et avec la profondeur spécifiée par glClearDepth)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix(); // on mémorise le repère courant


// POSITION CAMERA : passage Eye->World
  glTranslatef(0,0,-10);

 //  _trackball.applyGL(); // laissez en commentaire jusqu'à la question concernée


// Tracé de la scène par rapport à World
//  drawGround(); // laissez en commentaire jusqu'à la question concernée

  drawSquare();

  glPushMatrix();

  glPopMatrix();


  glPopMatrix(); // on restitue le repère courant (on retrouve le repère initial sur la caméra
}




