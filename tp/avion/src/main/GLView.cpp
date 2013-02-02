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
    _pressLeft=_pressRight=_pressUp=_pressDown=_pressAccelerate=_pressDecelerate=false; // arrows key
    _pressRollLeft=_pressRollRight=false;
    _angle=0;

    _kIncrement=0.01;
    _kInterpolate=0.2;
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

    UtilGL::init(this);

    // common OpenGL set up
    glEnable(GL_DEPTH_TEST);  // hidden parts removal with depth buffer algorithm
    glClearColor(1,1,1,0); // color value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done
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
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // les polygones seront tracés sans remplissage


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
        _trackball.motionXY(event->x(),event->y());
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
        case Qt::Key_W:
            _pressUp=true;
            break;
            // TODO
        case Qt::Key_S:
            _pressDown=true;
            break;
        case Qt::Key_A:
            _pressRollLeft=true;
            break;
        case Qt::Key_D:
            _pressRollRight=true;
            break;
        case Qt::Key_Q:
            _pressLeft=true;
            break;
        case Qt::Key_E:
            _pressRight=true;
            break;
        case Qt::Key_Space:
            _kIncrement=-_kIncrement;
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
        case Qt::Key_W:
            _pressUp=false;
            break;
            // TODO
        case Qt::Key_S:
            _pressDown=false;
            break;
        case Qt::Key_A:
            _pressRollLeft=false;
            break;
        case Qt::Key_D:
            _pressRollRight=false;
            break;
        case Qt::Key_Q:
            _pressLeft=false;
            break;
        case Qt::Key_E:
            _pressRight=false;
            break;
        default:
            QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
        }
}



/** ***************************************************************************
  init/update data
  **/
void GLView::initData() {
    // cameras setup : pour dernier exercice
    //_avion.read("/Users/benjamin/Documents/Education/M1/M3DS/tp/avion/media/biplane.obj");
   _avion.read("/Users/WannaGetHigh/workspace/M1S2/M3DS/tp/avion/media/biplane.obj");
    _cam1.position(Vector3(-2,4,10));
    _cam2.position(Vector3(10,0,0));
    Quaternion q;
    q.setIdentity();
    q.rotate(-40,Vector3(1,1,0));
    _cam1.orientation(q);
    q.setIdentity();
    q.rotate(90,Vector3(0,1,0));
    _cam2.orientation(q);
    //

}

void GLView::updateData() {
    // modifications des données apportées entre chaque image tracée :

    // A décommenter pour la prise en compte des touches claviers sur les données de l'avion
    if (_pressRollLeft) _avion.rollLeft();
    if (_pressRollRight) _avion.rollRight();
    if (_pressUp) _avion.pitchUp();
    if (_pressDown) _avion.pitchDown();
    if (_pressLeft) _avion.yawLeft();
    if (_pressRight) _avion.yawRight();
    if (_pressAccelerate) {_avion.accelerate();_pressAccelerate=false;}
    if (_pressDecelerate) {_avion.decelerate();_pressDecelerate=false;}

    _avion.move();

    _kInterpolate+=_kIncrement;
    if (_kInterpolate<0) _kInterpolate=0;
    if (_kInterpolate>1) _kInterpolate=1;

    _cam1.position(_avion.position() + _avion.orientation() * Vector3(0,0.4,-2));
    _cam1.orientation( _avion.orientation());

    _cam2.lookAt(_avion.position(), Vector3(0,-1,0));

    _camera.interpolate(_cam1, _cam2, _kInterpolate);

    updateGL();
}


/** ******************************************************************* **/
/**
  Drawings
**/



void GLView::paintGL() {
    // effacer écran (chaque pixel est affecté avec la couleur spécifiée par glClearColor, et avec la profondeur spécifiée par glClearDepth)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix(); // on mémorise le repère courant

    // glTranslatef(0,0,-5); // OpenGL modifie le repère courant : "avance" pour le mettre derrière l'écran
    //gluLookAt(2,5,75,_avion.position().x(),_avion.position().y(),_avion.position().z(),0,1,0);
    _camera.applyGL();
    _trackball.applyGL();
    _avion.drawWorld();

    glColor3f(0,0,0); // couleur noire
    UtilGL::drawGround();

    glPopMatrix(); // on restitue le repère courant
}




