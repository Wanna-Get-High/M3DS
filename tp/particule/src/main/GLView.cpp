/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <QtGui>
#include <iostream>
#include <sstream>

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
 initialize GL context (with glew) + common set up
 **/
void GLView::initializeGL() {

  // initialization of all available OpenGL functionalities
  if (glewInit()!=GLEW_OK) {
    cout << "glew error : unable to initialize glew" << endl;
    exit(EXIT_FAILURE);
  }
  cout << "GL version :" << glGetString(GL_VERSION) << endl;
  cout << "Shading Language  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);

  // common OpenGL set up
  glEnable(GL_DEPTH_TEST);  // hidden parts removal with depth buffer algorithm
  glClearColor(1,1,1,0);
  glClearDepth(1.0);

  // TODO
  // lighting setup
  glEnable(GL_LIGHTING);
  UtilGL::materialBlueGreen();
  UtilGL::initLight(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
  glEnable(GL_NORMALIZE);

  // drawing polygons set up
  glShadeModel(GL_SMOOTH); // activation remplissage avec interpolation linéaire (Gouraud)
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // les polygones seront tracés sans remplissage

  // all drawings will be black (till the next glColor3f instruction)
  glColor3f(0,0,0);

  glLightfv(GL_LIGHT0,GL_POSITION,(float []){0,0,1,0}); // vecteur d'éclairement L. Dans le vertex/fragment shader récupéré par gl_LightSource[0].position.xyz qui sera (0,0,1).


  initData();
}


/** ***************************************************************************
resize
  **/


/// when the graphics window is sized/resized (including the first time the window is shown)
void GLView::resizeGL(int width, int height) {
  glViewport(0,0,width,height);
  _camera.viewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.1,0.1,-0.1,0.1,.3,1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/** ***************************************************************************
  init/update data
  **/


void GLView::initProjection() {

  // définition de la matrice de transformation (identité = repère courant sur Eye).
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.5,0.5,-0.5,0.5,0.4,100);
  glMatrixMode(GL_MODELVIEW);
}


void GLView::initData() {
  _modeParticle=true;
  _animate=true;
  initProjection();


//  camera.setViewport(0,0,512,512);
  _camera.position(Vector3(0,0,5));
  _camera.lookAt(Vector3(0,0,0),Vector3(0,1,0));

  //_texture.setLuminance(true);
  _texture.read("lightmap.png");
  _texture.setAlpha(Color(0,0,0),0,1.1);


  _particleList.birthRate(5);

  _particleList.texture(&_texture);
  _particleList.maxi(2000);

  _engine.particleList(&_particleList);

  // Ajout de plans (gérés par la détection de collision) :
  // le sol :
  _engine.addPlane(new Plane(Vector3(0,-2,0),Vector3(0,1,0)));

  // des "murs" (non visibles)

/*
  _engine.addPlane(new Plane(Vector3(-4,0,0),Vector3(1,0,0)));
  _engine.addPlane(new Plane(Vector3(4,0,0),Vector3(-1,0,0)));
  _engine.addPlane(new Plane(Vector3(0,0,-4),Vector3(0,0,1)));
  _engine.addPlane(new Plane(Vector3(0,0,4),Vector3(0,0,-1)));
*/



  _ground.read("mosaic_pierre.jpg");
  _ground.generateMipmap();

}

void GLView::updateData() {
  if (_pressForward) _camera.forward(0.1);
  if (_pressBackward) _camera.backward(0.1);
  if (_pressLeft) _camera.left(0.1);
  if (_pressRight) _camera.right(0.1);
  if (_mouseRightButton) {
    bool ok=_camera.mouseToRayWorld(_mouseScreen,&_windRay);
    cout << "windRay = " << _windRay << " mouse = " << _mouseScreen << endl;
    _engine.enableWind(_windRay);
  }
  else {
    _engine.disableWind();
  }
  if (_animate) _engine.update();

  updateGL();
}


/** **********************************************************************
  Events
  **/
void GLView::mousePressEvent(QMouseEvent *event) {
  if (event->button()==Qt::LeftButton) {
    _trackball.start(event->x(),event->y());
    _mouseLeftButton=true;
  }
  if (event->button()==Qt::RightButton) {
    _mouseScreen=Vector2(event->x(),event->y());
    _mouseRightButton=true;
  }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  if (_mouseLeftButton) {
    _trackball.motionXY(event->x(),event->y());
  }
  else if (_mouseRightButton) {
    _mouseScreen=Vector2(event->x(),event->y());
  }
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
  switch(event->button()){
  case Qt::LeftButton:
    _trackball.stop();

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
    case Qt::Key_P:
      _modeParticle=!_modeParticle;
      _engine.modeParticle(_modeParticle);
      break;
    case Qt::Key_A:
      _animate=!_animate;
      _particleList.startTime();
      break;
    case Qt::Key_S:
      _pressBackward=true;
      break;
    case Qt::Key_Z:
      _pressForward=true;
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
      break;
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



void GLView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.5,0.5,-0.5,0.5,0.4,20000);
  _camera.frustum(-0.5,0.5,-0.5,0.5,0.4,20000);
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  _camera.applyGL();
  _trackball.applyGL();

  glPushMatrix();
  glTranslatef(0,-2,10);
  UtilGL::drawGround(_ground);
  glPopMatrix();


  _engine.draw();

  glPopMatrix();
}





