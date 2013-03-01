/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <iostream>
#include <QtEvents>
#include "Error.h"

using namespace std;
using namespace prog3d;

/// ctor
GLView::GLView(const QGLFormat &format,QWidget *parent) : QGLWidget(format,parent) {

  setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events

  // Data initializations
  _mouseLeftButton=false; // left button is pressed ?
  _mouseRightButton=false; // right button is pressed ?
  _pressLeft=_pressRight=_pressUp=_pressDown=_pressAccelerate=_pressDecelerate=false; // arrows key
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
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  glEnable(GL_DEPTH_TEST);  // hidden parts removal with depth buffer algorithm
  glClearColor(1,1,1,0); // color value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done
  glClearDepth(1.0);       // depth value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done

  // TODO
  // lighting setup
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
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
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // les polygones seront tracés sans remplissage

  // this exercice begins without lighting :
//  glDisable(GL_LIGHTING); // on désactive l'éclairement

  // all drawings will be black (till the next glColor3f instruction)
  glColor3f(0,0,0);



  _tex0.read("../media/briques02.jpg");
  _square.texture(0,_tex0.id());

  _tex1.read("../media/lightmap.png");
  _square.texture(1,_tex1.id());

  _tex2.read("../media/univ_lille1pair.jpg");
  _square.texture(2,_tex2.id());


  _shaderSimple.read("../media/shader/simple");
  _shaderGooch.read("../media/shader/gooch");
  _shaderOutline.read("../media/shader/outline");
  _shaderCel.read("../media/shader/cel");
  _shaderLight.read("../media/shader/light");



  _square.initBuffer();
  _cow.initBuffer();

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
  _zCam=6.0;
  _modeDisplay=1;

  try {
  _cow.read("../media/cow.obj");
  }
  catch (Error &e) {
    throw ErrorD("cant read cow.obj");
  }

  _nbCellStep=1;





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
    case Qt::Key_5:
      _modeDisplay=5;
      break;
    case Qt::Key_Z:
      _pressUp=true;
      break;
    case Qt::Key_S:
      _pressDown=true;
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
    default:
      QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
    }
}


/** ******************************************************************* **/
/**
  Drawings
**/

void GLView::drawSquare() {
  glColor3f(0,0.6,0.5);
  UtilGL::drawText("Basic shader (+ multi-texture)",Vector2(0,0));

  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  _shaderSimple.enable();

  // LocationImage contient alors un "identifiant" pour la variable "image" du shader.
  GLuint locationImage = glGetUniformLocation(_shaderSimple.id(),"image");

  // On affecte à la variable image du shader la valeur 0 (i.e. l'unité de texture 0).
  glUniform1i(locationImage, 0);


  _square.drawBuffer();
  _shaderSimple.disable();
}


void GLView::drawCowGooch() {
  glColor3f(0,0.6,0.5);
  UtilGL::drawText("Gooch shader",Vector2(0,0));
  _shaderGooch.enable();
  _cow.drawBuffer();
  _shaderGooch.disable();
}

void GLView::drawCowOutline() {
  glColor3f(0,0.6,0.5);
  UtilGL::drawText("Outline shader (with gooch shader)",Vector2(0,0));
  _shaderOutline.enable();
  _cow.drawBuffer();
  _shaderOutline.disable();
}


void GLView::drawCowLight() {
  glColor3f(0,0.6,0.5);

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

  UtilGL::drawText("Lighting shader",Vector2(0,0));
  _shaderLight.enable();
  _cow.drawBuffer();
  _shaderLight.disable();
}

void GLView::drawCowCel() {
  glColor3f(0,0.6,0.5);
  UtilGL::drawText("Cel shader",Vector2(0,0));
  _shaderCel.enable();
  _cow.drawBuffer();
  _shaderCel.disable();
}




void GLView::paintGL() {

  glClearColor(1,1,1,0);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

  glColor3f(1,0,0);

  glPushMatrix();

  glTranslatef(0,0,-_zCam);
  _trackball.applyGL();

  switch (_modeDisplay) {
  case 1:drawSquare();break;
  case 2:drawCowLight();break;
  case 3:drawCowCel();break;
  case 4:drawCowGooch();break;
  case 5:drawCowOutline();break;
    default:break;
  }

  glPopMatrix();
  glUseProgram(0);

  glColor3f(0,0,0);
}





