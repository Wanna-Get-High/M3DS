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

  // Data initializations
  _mouseLeftButton=false; // left button is pressed ?
  _mouseRightButton=false; // right button is pressed ?
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
  if (glewInit() != GLEW_OK) {
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
  glDisable(GL_LIGHTING); // on désactive l'éclairement

  // all drawings will be black (till the next glColor3f instruction)
  glColor3f(0,0,0);

  UtilGL::init(this);

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
  _width=width;_height=height;
}


/** ***************************************************************************
  init/update data
  **/
void GLView::initData() {

  _modeDraw=DRAW_HERMITE;
  _modeMouse=NO_MOTION;
  _modeControl=CONTROL_HERMITE;
  _viewControlPoint=0;

  _interactBezier.data(_bezier.pointv());
  _interactCatmullRom.data(_catmullRom.pointv());
  _interactCatmullRom.appendData(_catmullRom.intermediatev());
  _interactHermite.createData();

  _whichInteraction=&_interactHermite;
}

void GLView::updateData() {
  initProjection();
  updateGL();
}


/** **********************************************************************
  Events
  **/
void GLView::mousePressEvent(QMouseEvent *event) {
  Vector2 mouse(event->x(),event->y());
  if (event->button()==Qt::LeftButton) {
      cout << "left" << endl;
    _mouseLeftButton=true;
    _modeMouse=TRACKBALL;

    // TODO
    _trackball.start(event->x(),event->y());

  }
  if (event->button()==Qt::RightButton) {
      cout << "right" << endl;
       _whichInteraction->selectNearest(mouse,10);
       _modeMouse=MOVE_POINT;
       _mouseRightButton=true;
  }
  if (event->button()==Qt::MidButton) {
      cout << "middle" << endl;
    if (_modeDraw==DRAW_BEZIER) {
      Vector3 add=_interactBezier.addEnd(mouse);
      _bezier.addEnd(add);
      _interactBezier.data(_bezier.pointv());
    } else if (_modeDraw==DRAW_CATMULL) {
      _interactCatmullRom.data(_catmullRom.pointv());
      Vector3 add=_interactCatmullRom.addEnd(mouse);
      _catmullRom.addEnd(add);
      _interactCatmullRom.data(_catmullRom.pointv());
      _interactCatmullRom.appendData(_catmullRom.intermediatev());
    } else if (_modeDraw==DRAW_HERMITE) {
      if (_interactHermite.size()>4) {
       _interactHermite.createData();
      }
      _interactHermite.addEnd(mouse);
   }
  }

}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  if (_modeMouse==TRACKBALL) {
    _trackball.motion(event->x(),event->y());
   }
   if (_modeMouse==MOVE_POINT) {

     _whichInteraction->controlSelected(Vector2(event->x(),event->y()));
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
    _modeMouse=NO_MOTION;
    _mouseRightButton=false;
    break;
  default:break;
  }
}

void GLView::wheelEvent(QWheelEvent *event) {
  cout << "wheel event" << event->delta() << endl;
}


void GLView::keyPressEvent(QKeyEvent *event) {
  QPoint p=this->mapFromGlobal(QCursor::pos());
  Vector2 mouse(p.x(),p.y());

  if (event->isAutoRepeat())
    QGLWidget::keyPressEvent(event);
  else
    switch(event->key()){
     case Qt::Key_A:
       if (_modeDraw==DRAW_BEZIER) {
         Vector3 add=_interactBezier.addEnd(mouse);
         _bezier.addEnd(add);
         _interactBezier.data(_bezier.pointv());
       }
       break;
     case Qt::Key_I:
       if (_modeDraw==DRAW_BEZIER) {
         int where;
         Vector3 add=_interactBezier.insertNearest(mouse,10,&where);
         if (where>=0) {
           _bezier.insert(add,where);
           _interactBezier.data(_bezier.pointv());
         }
       }
       break;
     case Qt::Key_F:
       if (_modeControl==CONTROL_CATMULL) {
         _catmullRom.setup();
       }
       break;
     case Qt::Key_C:
       if (_modeDraw==DRAW_BEZIER) _viewControlPoint=(_viewControlPoint+1)%2;
       else _viewControlPoint=(_viewControlPoint+1)%2;
       break;
    case Qt::Key_1:
      _modeDraw=DRAW_HERMITE;
      _modeControl=CONTROL_HERMITE;
       _whichInteraction=&_interactHermite;
       break;
     case Qt::Key_2:
       _modeDraw=DRAW_BEZIER;
       _modeControl=CONTROL_BEZIER;
       _whichInteraction=&_interactBezier;
       _interactBezier.data(_bezier.pointv());
       break;
     case Qt::Key_3:
       _modeDraw=DRAW_CATMULL;
       _modeControl=CONTROL_CATMULL;
       _whichInteraction=&_interactCatmullRom;
       _interactCatmullRom.data(_catmullRom.pointv());
       _interactCatmullRom.appendData(_catmullRom.intermediatev());
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
    default:
      QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
    }
}


/** ******************************************************************* **/
/**
  Drawings
**/


/** ******************************************************************************************************** */
/**
  Init projection
*/
void GLView::initProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLView::drawBezier() {
  glLineWidth(3);
  _interactBezier.setView();
  if (_viewControlPoint==0) {
    glColor3f(0,0,1);
    _bezier.drawControl();
  }
  else if (_viewControlPoint==1) {
    glColor3f(0,0,1);
    _bezier.drawPoint();
  }
  glColor3f(0.8,0.2,0.3);
  _bezier.drawBezier();
  glLineWidth(1);
}


void GLView::drawCatmullRom() {
  _interactCatmullRom.setView();

  _catmullRom.drawControl(3-_viewControlPoint);
  glColor3f(1,0,0);
  _catmullRom.draw();
}

void GLView::drawHermite() {
  _interactHermite.setView();
  glColor3f(1,0,0);

  Vector3 a(0,0,0);
  Vector3 b(0,0,0);
  Vector3 na,nb;


  glColor3f(0,0,1);
  switch(_interactHermite.size()) {
  case 4:
    nb.set(_interactHermite.point(2),_interactHermite.point(3));
    UtilGL::drawArrow(_interactHermite.point(2),nb,0.01,"","N_B");
  case 3:
    b.set(_interactHermite.point(2));
    UtilGL::draw("B",b+Vector3(0.02,0.02,0.0));
    UtilGL::draw(b);
  case 2:
    na.set(_interactHermite.point(0),_interactHermite.point(1));
    UtilGL::drawArrow(_interactHermite.point(0),na,0.01,"","N_A");
  case 1:
    a.set(_interactHermite.point(0));
    UtilGL::draw("A",a+Vector3(0.02,0.02,0.0));
    UtilGL::draw(a);

  }



  if (_interactHermite.size()==4) {
    glColor3f(1,0,0);
    _hermite.set(a,na*5.0,b,nb*5.0);
    glLineWidth(3);
    _hermite.draw();
    glLineWidth(1);
  }
}

void GLView::printMode() {
  switch (_modeDraw) {
  case DRAW_BEZIER:
    UtilGL::drawText("Bezier",Vector2(0,0));
    break;
  case DRAW_CATMULL:
    UtilGL::drawText("Catmull Rom",Vector2(0,0));
    break;
  case DRAW_HERMITE:
    UtilGL::drawText("Hermite",Vector2(0,0));
    break;
  }
}


void GLView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  printMode();

  switch (_modeDraw) {
  case DRAW_BEZIER:
    drawBezier();
    break;
  case DRAW_CATMULL:
    drawCatmullRom();
    break;
  case DRAW_HERMITE:
    drawHermite();
    break;
  default:
    break;
  }

  glPopMatrix();
}




