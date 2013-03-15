/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <QtEvents>
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
  _pressLeft=_pressRight=_pressUp=_pressDown=_pressAccelerate=_pressDecelerate=_pressRollLeft=_pressRollRight=false; // arrows key
}


/// dtor
GLView::~GLView() {
  UtilGL::kill();
  delete[] _wingedObject;
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

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);


  UtilGL::materialFrontBack();
  UtilGL::initLight(GL_LIGHT0);

  glLightfv(GL_LIGHT0,GL_POSITION,(float []){0,0,1,0}); // vecteur d'éclairement L. Dans le vertex/fragment shader récupéré par gl_LightSource[0].position.xyz qui sera (0,0,1).
  glMaterialfv(GL_FRONT,GL_DIFFUSE,(float []){0,0.8,0,0}); // coefficient Kd du diffus. (rouge,vert,bleu,alpha) = (0,0.8,0,0). Récupéré dans le vertex/fragment shader par gl_FrontMaterial.diffuse
  glMaterialfv(GL_FRONT,GL_SPECULAR,(float []){0,0.2,0.8,0}); // coefficient Ks du spéculaire. (rouge,vert,bleu,alpha) = (0,0.2,0.8,0). Récupéré dans le vertex/fragment shader par gl_FrontMaterial.specular
  glMateriali(GL_FRONT,GL_SHININESS,50); // Brillance = 50. Récupéré dans le vertex/fragment shader par gl_FrontMaterial.shininess

  initData();
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

  _nbObject=3;
  _wingedObject=new Winged[3];

  _visuPoint=false;
  _visuBoundary=false;
  _visuHide=0;
  _computeVertexNormal=false;
  _drawLineCatmull=false;
  _boundary.clear();
  _active=0;

  _ground.generateMipmap(true);
  _ground.read("mosaic_pierre.jpg");
  _ground.filterMipmap();

  _camera.position(Vector3(0,2,4));



  _wingedObject[0].read("002_cube.obj");
  _wingedObject[1].read("cow.obj");
  _wingedObject[2].read("al.obj");

}

void GLView::updateData() {
  if (_pressUp) _camera.forward(0.2);
  if (_pressDown) _camera.backward(0.2);
  if (_pressLeft) _camera.left(0.2);
  if (_pressRight) _camera.right(0.2);
  if (_pressRollLeft) _camera.rotate(0.9,Vector3(0,1,0));
  if (_pressRollRight) _camera.rotate(-0.9,Vector3(0,1,0));

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
     case Qt::Key_C:
       _wingedObject[_active].catmullClark();
       _wingedObject[_active].computeNormalVertex();
       _wingedObject[_active].stats();
       break;
     case Qt::Key_V:
       _wingedObject[_active].computePointFace();

       _wingedObject[_active].computePointEdge();

       _wingedObject[_active].computePointVertex();

       break;
     case Qt::Key_P:
       _visuPoint=!_visuPoint;
       break;
     case Qt::Key_B:
       _visuBoundary=!_visuBoundary;
       if (_visuBoundary) _boundary=_wingedObject[_active].boundary();

       break;
     case Qt::Key_N:
       _wingedObject[_active].computeNormalVertex();
       break;
     case Qt::Key_G:
       _visuHide=(_visuHide+1)%3;
       break;
     case Qt::Key_H:
       _drawLineCatmull=!_drawLineCatmull;
       break;
     case Qt::Key_Less:
       _active=(_active+1)%_nbObject;
       _visuBoundary=false;
       break;
     case Qt::Key_Greater:
       if (_active==0) _active=_nbObject-1;
       else _active=(_active-1);
       _visuBoundary=false;
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


void GLView::drawWinged() {
  UtilGL::materialBlueGreen();
  glLineWidth(2);
  glColor3f(1,1,1);
  if (_drawLineCatmull) {
    _wingedObject[_active].drawLineCatmull();
  }
  else {
    switch(_visuHide) {
    case 0:
      _wingedObject[_active].drawWire(Color(0,0,0));
      break;
    case 1:
      _wingedObject[_active].drawWireHide(Color(0,0,0));
      break;
    case 2:
      _wingedObject[_active].draw();
      break;
    }
  }
  if (_visuPoint) {
    glColor3f(0,0,1);
    _wingedObject[_active].drawPointFace();
    glColor3f(1,0,0);
    _wingedObject[_active].drawPointEdge();
    glColor3f(0,1,0);
    _wingedObject[_active].drawPointVertex();
  }
}


void GLView::paintGL() {
  glClearColor(1,1,1,0);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

  glPushMatrix();

  _camera.applyGL();

  UtilGL::drawGround(_ground);


  glTranslatef(0,2,-2);
  _trackball.applyGL();

  if (_visuBoundary) {
    glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT);
    glDisable(GL_LIGHTING);
    glLineWidth(4);
    glColor3f(1,0,0);
    for(unsigned int i=0; i<_boundary.size(); i++) {
      _boundary[i]->drawLine();
    }
    glPopAttrib();
  }
  drawWinged();


  glPopMatrix();


}





