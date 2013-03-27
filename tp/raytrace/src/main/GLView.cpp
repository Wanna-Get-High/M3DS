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

  // common OpenGL set up
  glEnable(GL_DEPTH_TEST);  // hidden parts removal with depth buffer algorithm
  glClearColor(1,1,1,0);
  glClearDepth(1.0);

  // TODO
  // lighting setup
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  UtilGL::materialFrontBack();
  glLightfv(GL_LIGHT0,GL_AMBIENT,(float []){0.8,0.8,0.8,0.0});
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
  glEnable(GL_NORMALIZE);
  //glEnable(GL_COLOR_MATERIAL);
  //glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);

  // drawing polygons set up
  glShadeModel(GL_SMOOTH); // activation remplissage avec interpolation linéaire (Gouraud)
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // les polygones seront tracés sans remplissage

  // this exercice begins without lighting :
//  glDisable(GL_LIGHTING); // on désactive l'éclairement

  // all drawings will be black (till the next glColor3f instruction)
  glColor3f(0,0,0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  float pp[]={1,1,1,1};
  for(unsigned int i=0;i<8;i++) {
    glLightfv(GL_LIGHT0+i,GL_AMBIENT,pp);
    glLightfv(GL_LIGHT0+i,GL_DIFFUSE,pp);
    glLightfv(GL_LIGHT0+i,GL_SPECULAR,pp);
  }

  UtilGL::materialFrontBack();
  UtilGL::initLight(GL_LIGHT0);

  glLightfv(GL_LIGHT0,GL_POSITION,(float []){0,0,1,0}); // vecteur d'éclairement L. Dans le vertex/fragment shader récupéré par gl_LightSource[0].position.xyz qui sera (0,0,1).
  glMaterialfv(GL_FRONT,GL_DIFFUSE,(float []){0,0.8,0,0}); // coefficient Kd du diffus. (rouge,vert,bleu,alpha) = (0,0.8,0,0). Récupéré dans le vertex/fragment shader par gl_FrontMaterial.diffuse
  glMaterialfv(GL_FRONT,GL_SPECULAR,(float []){0,0.2,0.8,0}); // coefficient Ks du spéculaire. (rouge,vert,bleu,alpha) = (0,0.2,0.8,0). Récupéré dans le vertex/fragment shader par gl_FrontMaterial.specular
  glMateriali(GL_FRONT,GL_SHININESS,50); // Brillance = 50. Récupéré dans le vertex/fragment shader par gl_FrontMaterial.shininess

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
  init/update data
  **/

void GLView::initData() {
  _camera.position(Vector3(0,0,5));
}

void GLView::updateData() {
  if (_pressForward) _camera.forward(0.2);
  if (_pressBackward) _camera.backward(0.2);
  if (_pressLeft) _camera.left(0.2);
  if (_pressRight) _camera.right(0.2);
  if (_pressLeft) _camera.rotate(0.1,Vector3(0,0,1));
  if (_pressRight) _camera.rotate(-0.1,Vector3(0,0,1));

  updateGL();
}


/** **********************************************************************
  Events
  **/
void GLView::mousePressEvent(QMouseEvent *event) {

  if (event->button()==Qt::LeftButton) {
    _mouseLeftButton=true;

    // TODO
    _trackball.start(event->x(),event->y());

  }
  if (event->button()==Qt::RightButton) {
    _mouseRightButton=true;
  }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  if (_mouseLeftButton) {
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
  if (event->delta()<0) _pressDecelerate=true;
  else _pressAccelerate=true;
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



void GLView::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//    glViewport(0,0,width()/2,height());

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(60,1,0.1,100);
//    glFrustum(-1,1,-1,1,.5,100);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    _camera.applyGL();
    _trackball.applyGL();
    _scene->drawGL();
    glColor3f(1,1,1);
    UtilGL::drawText(_scene->name(),Vector2(0,0));
    glPopMatrix();


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}





