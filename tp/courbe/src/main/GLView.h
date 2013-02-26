/**
*
*  @author F. Aubert
*  @file GLView
*
*/



#ifndef GLVIEW_H
#define GLVIEW_H

#include "glsupport.h"
#include "Trackball.h"
#include "UtilGL.h"
#include <QtGui>
#include "Hermite.h"
#include "Curve.h"
#include "CatmullRomCurve.h"
#include "ControlVector3.h"

/**
  @class GLView

  The widget to draw with OpenGL.
  */


typedef enum {DRAW_BEZIER,DRAW_CATMULL,DRAW_HERMITE} EDraw;
typedef enum {CONTROL_BEZIER,CONTROL_CATMULL,CONTROL_HERMITE} EControl;
typedef enum {TRACKBALL,MOVE_POINT,NO_MOTION} EMouse;


class GLView : public QGLWidget {
  Q_OBJECT
public:
  //! Ctor
  explicit GLView(const QGLFormat &format,QWidget *parent = 0);
  //! Dtor
  virtual ~GLView();

  //! initialize data application
  void initData();



protected:
  // OpenGL drawings

  //! initialize the OpenGL context
  void initializeGL();
  //! OpenGL drawing (called when updateGL() )
  void paintGL();
  //! should update projection (called when the widget is resized)
  void resizeGL(int width,int height);

  // events
  //! called when a mouse button is pressed in the widget
  void mousePressEvent(QMouseEvent *event);
  //! called when the mouse is moved in the widget
  void mouseMoveEvent(QMouseEvent *event);
  //! called when a mouse button is released in the widget
  void mouseReleaseEvent(QMouseEvent *event);
  //! called when a key is pressed in the widget
  void keyPressEvent( QKeyEvent *event );
  //! called when a key is released in the widget
  void keyReleaseEvent( QKeyEvent *event );
  //! called when the mouse wheel is turned
  void wheelEvent(QWheelEvent *event);

private:
  bool _mouseLeftButton; //! true when mouse left is pressed
  bool _mouseRightButton; //! true when mouse right is pressed
  int _width,_height;

  /** TP ************************ */

  EDraw _modeDraw;
  EMouse _modeMouse;
  EControl _modeControl;

  Hermite _hermite;
  Curve _bezier;
  CatmullRomCurve _catmullRom;

  prog3d::ControlVector3 _interactBezier;
  prog3d::ControlVector3 _interactCatmullRom;
  prog3d::ControlVector3 _interactHermite;

  prog3d::ControlVector3 *_whichInteraction;

  unsigned int _viewControlPoint;


  prog3d::Trackball _trackball; // contrôle du placement de la scène à la souris

  void initProjection();
  void drawHermite();
  void drawBezier();
  void drawCatmullRom();
  void printMode();

  /** TP ************************ */

signals:

public slots:
  //! called every 20ms by the MainWindow
  void updateData();

};

#endif // GLVIEW_H
