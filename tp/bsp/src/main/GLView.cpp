/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <QtGui>
#include <iostream>

using namespace std;
using namespace prog3d;

static double myRand(double binf,double bsup) {
  return double(rand())/RAND_MAX*(bsup-binf)+binf;
}

/// ctor
GLView::GLView(const QGLFormat &format,QWidget *parent) : QGLWidget(format,parent) {

  setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events
  UtilGL::init(this);

  // Data initializations
  _mouseLeftButton=false; // left button is pressed ?
  _mouseRightButton=false; // right button is pressed ?
  _pressLeft=_pressRight=_pressUp=_pressDown=_pressAccelerate=_pressDecelerate=false; // arrows key
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
  glClearColor(1,1,1,0); // color value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done
  glClearDepth(1.0);       // depth value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done

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

  _nbSolution=3;
  _zCam=6.0;
  _modeDisplay=1;
  _solution=0;

  srand(time(NULL));

  // pour test signe et intersection
  _bsp1.read("media/une_face.obj");

  // pour test de découpe sur 2 faces
  _bsp2.read("media/deux_face.obj");
  _bsp2.face(0)->separate(*_bsp2.face(1));

  // pour test simple de découpe

  // TODO : à changer
  _bsp3.read("media/simple_nontri.obj");
}

void GLView::updateData() {
  if (_pressUp) _zCam-=0.05;
  if (_pressDown) _zCam+=0.05;

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
    case Qt::Key_1:
      _modeDisplay=1;
      break;
    case Qt::Key_2:
      _modeDisplay=2;
      break;
    case Qt::Key_3:
      _modeDisplay=3;
      break;
    case Qt::Key_4:
      _modeDisplay=4;
      break;
    case Qt::Key_Z:
      _pressUp=true;
      break;
      // TODO
    case Qt::Key_S:
      _pressDown=true;
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
    case Qt::Key_B:
      cout << "BSP construction" << endl;
      cout << "Number of faces = " << _bsp3.nbFace() << endl;
      _bsp3.createBSP();
      cout << "After creation : number of faces = " << _bsp3.nbFace() << endl;
      break;
    case Qt::Key_Space:
      _solution=(_solution+1)%_nbSolution;
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
      _pressUp=false;
      break;
      // TODO
    case Qt::Key_S:
      _pressDown=false;
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


/** ******************************************************************* **/
/**
  Drawings
**/

void GLView::display1() {

  // Test du signe (génération brutale à chaque passage => uniquement pour tester).
  _bsp1.drawDepth();
  srand(1);
  UtilGL::drawText("Testing face sign",Vector2(0,0));
  glDisable(GL_LIGHTING); // pour pouvoir controler la couleur des points
  for(int i=0; i<100; i++) {
    Vector3 p;
    p.x(myRand(-1.0,1.0));
    p.y(myRand(-1.0,1.0));
    p.z(myRand(-1.0,1.0));
    FaceBSP *f=_bsp1.face(0);
    ESign s=f->sign(p);
    switch (s) {
    case SIGN_NONE:
      glColor3f(0,0,1);
      break;
    case SIGN_PLUS:
      glColor3f(0,1,0);
      break;
    case SIGN_MINUS:
      glColor3f(1,0,0);
      break;
    }
    UtilGL::draw(p);
  }
  glEnable(GL_LIGHTING);
}

void GLView::display2() {
  glColor3f(1,0,0);
  _bsp1.drawDepth();
  srand(1);
  UtilGL::drawText("Testing face intersection",Vector2(0,0));
  for(int i=0; i<10; i++) {
    Vector3 p1,p2;
    p1.set(myRand(-2.0,0.0),myRand(-1.0,1.0),myRand(0.0,2.0));
    p2.set(myRand(0.0,2.0),myRand(-1.0,1.0),myRand(-2.0,0.0));
    Vector3 p=_bsp1.face(0)->intersection(p1,p2);
    glColor3f(0,0.2,0.5);
    UtilGL::draw(p1,p2,2);
    glColor3f(0.6,0.2,1);
    UtilGL::draw(p,10);
  }
}

void GLView::display3() {
  UtilGL::drawText("Testing face cutting",Vector2(0,0));
  glColor3f(1,0,0);
  _bsp2.drawDepth();
  glColor3f(0,0,1);
}


void GLView::display4() {
  if (_bsp3.bsp()==0) {
    glColor3f(1,0,0);
    UtilGL::drawText("Press 'B' to build the BSP...",Vector2(0.2,0.5));
  }
  else {
  switch (_solution) {
  case 0:
    UtilGL::drawText("Render BSP without hidden removal",Vector2(0,0));
    _bsp3.drawBrut();
    break;
  case 1:
    UtilGL::drawText("Render BSP with depth buffer",Vector2(0,0));
    glColor3f(1,0,0);
    _bsp3.drawDepth();
    break;
  case 2:
    UtilGL::drawText("Render BSP with painter algorithm",Vector2(0,0));
    glColor3f(0,0.8,0.2);
    _bsp3.drawBSP(_obs);
    break;
  }
  }
}



void GLView::paintGL() {
  glClearColor(1,1,1,1);
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float pos[4]= {0,0,1,0};
  glLightfv(GL_LIGHT0,GL_POSITION,pos);

  glPushMatrix();

  glTranslatef(0,0,-_zCam);
  _trackball.applyGL();

  _obs=Vector3(0,0,_zCam);
  _obs=conjugate(_trackball.quaternion())*_obs;


  switch (_modeDisplay) {
  case 1:

    display1();
    break;
  case 2:
    display2();
    break;
  case 3:
    display3();
    break;
  case 4:
    display4();
    break;
  }
  glPopMatrix();
}




