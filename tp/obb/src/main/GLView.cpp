/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <iostream>
#include <sstream>
#include <QtGui>

using namespace std;
using namespace prog3d;

/// ctor
GLView::GLView(const QGLFormat &format,QWidget *parent) : QGLWidget(format,parent) {

  setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events

  // Data initializations
  _mouseLeftButton=false; // left button is pressed ?
  _mouseRightButton=false; // right button is pressed ?
  _pressLeft=_pressRight=_pressForward=_pressBackward=_pressAccelerate=_pressDecelerate=false; // arrows key
  UtilGL::init(this);
}


/// dtor
GLView::~GLView() {
  UtilGL::kill();
}

/**
 initialize GL context + common set up
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
  glClearColor(1,1,1,0);
  glClearDepth(1.0);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // les polygones seront tracés sans remplissage

  glColor3f(0,0,0);

  initData();
}


/** ***************************************************************************
resize
  **/


/// when the graphics window is sized/resized (including the first time the window is shown)
void GLView::resizeGL(int width, int height) {
  // window to world
  _frustumLeft=-10;
  _frustumRight=10;
  _frustumBottom=-10;
  _frustumTop=10;
  _frustumNear=-1;_frustumFar=1;
  _viewportX=0;_viewportY=0;_viewportWidth=width;_viewportHeight=height;

  glViewport(_viewportX,_viewportY,_viewportWidth,_viewportHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(_frustumLeft,_frustumRight,_frustumBottom,_frustumTop,_frustumNear,_frustumFar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/** ***************************************************************************
  init/update data
  **/

void GLView::initData() {



  // ajout des plans de collisions (les bords de l'environnement)
  _engineBox.add(new Plane(Vector3(0,-7,0),Vector3(0,1,0)));
  _engineBox.add(new Plane(Vector3(-8,0,0),Vector3(1,0,0)));
  _engineBox.add(new Plane(Vector3(6,0,0),Vector3(-1,0,0)));
  _engineBox.add(new Plane(Vector3(0,9,0),Vector3(0,-1,0)));

  Texture *tex1=new Texture();
  tex1->read("Tyrol.jpg");
  tex1->filterLinear();
  Texture *tex2=new Texture();
  tex2->read("Tranquil_Lagoon.jpg");
  tex2->filterLinear();

  // Définition première boite
  Box *b=new Box();
//  b->dimension(3,2);
//  b->position(Vector3(-2,5,0));
//  b->theta(-0.3);
//  b->mass(1);
//  b->computeInertia();
//  b->texture(tex1);
//  b->color(Color(1,0,1));
//  _boxList.add(b);

  // Définition seconde boite
  b=new Box();
  b->dimension(10,2);
  b->position(Vector3(3,0,0));
  b->theta(0.5);
  b->mass(1);
  b->computeInertia();
  b->texture(tex2);
  b->color(Color(0,1,0));
  _boxList.add(b);

  // Définition troisieme boite
  b=new Box();
  b->dimension(6,2);
  b->position(Vector3(-2,5,0));
  b->theta(0.1);
  b->mass(2);
  b->computeInertia();
  b->texture(tex2);
  b->color(Color(0,1,0));
  _boxList.add(b);


  _engineBox.boxList(&_boxList);

}

void GLView::updateData() {
  _engineBox.update();
  updateGL();
}


/** **********************************************************************
  Events
  **/
void GLView::mousePressEvent(QMouseEvent *event) {
  if (event->button()==Qt::LeftButton) {
    _applyForce=true;
    _pointForce=toWorld(Vector2(event->x(),event->y()));
    _pointForce.z(0);
    _engineBox.activeCursor(_pointForce);
     _mouseLeftButton=true;
  }
  if (event->button()==Qt::RightButton) {
    _mouseRightButton=true;
  }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  if (_mouseLeftButton) {
    _pointForce=toWorld(Vector2(event->x(),event->y()));
    _pointForce.z(0);
    _engineBox.motionCursor(_pointForce);

  }
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
  switch(event->button()){
  case Qt::LeftButton:
    _applyForce=false;
    _engineBox.disableCursor();

    _mouseLeftButton=false;
    break;
  case Qt::RightButton:
    _mouseRightButton=false;
    break;
  default:break;
  }
}

void GLView::wheelEvent(QWheelEvent *event) {
  /*
  if (event->delta()<0) _pressDecelerate=true;
  else _pressAccelerate=true;
  */
}


void GLView::keyPressEvent(QKeyEvent *event) {
  if (event->isAutoRepeat())
    QGLWidget::keyPressEvent(event);
  else
    switch(event->key()){
    // TODO
    case Qt::Key_S:
      _pressBackward=true;
      break;
    case Qt::Key_Z:
      _pressForward=true;
      break;
    case Qt::Key_Q:
      _pressRollLeft=true;
      break;
    case Qt::Key_D:
      _pressRollRight=true;
      break;
    case Qt::Key_A:
      _pressLeft=true;
      break;
    case Qt::Key_E:
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
      break;
      // TODO
    case Qt::Key_S:
      _pressBackward=false;
      break;
    case Qt::Key_Q:
      _pressRollLeft=false;
      break;
    case Qt::Key_D:
      _pressRollRight=false;
      break;
    case Qt::Key_A:
      _pressLeft=false;
      break;
    case Qt::Key_E:
      _pressRight=false;
      break;
    default:
      QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
    }
}



// transforme coordonnées souris en coordonnées monde
// précondition : viewport = 512x512, projection = ortho -10,10,-10,10 (codage en dur).
Vector3 GLView::toWorld(const Vector2 &p) {
  Vector3 res;
  res.x(p.x()/_viewportWidth*(_frustumRight-_frustumLeft)+_frustumLeft);
  res.y((_viewportHeight-p.y())/_viewportHeight*(_frustumTop-_frustumBottom)+_frustumBottom);
  res.z(0);
  return res;
}




/** ******************************************************************* **/
/**
  Drawings
**/



void GLView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  _engineBox.draw();

  _engineBox.drawPlane2D();

  UtilGL::drawDebug(false);
  UtilGL::debugClear();

  if (_engineBox.isCursorActive()) {
    glDepthFunc(GL_ALWAYS);
    glColor3f(1,0,0);
    UtilGL::draw(_pointForce,(_boxList.selected())->attachWorld());
    glDepthFunc(GL_LESS);
  }



  glColor3f(0,0,0);

  glPopMatrix();
}





