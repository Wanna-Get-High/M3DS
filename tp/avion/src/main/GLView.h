/**
*
*  @author F. Aubert
*  @file GLView
*
*/



#ifndef GLVIEW_H
#define GLVIEW_H

#include "glsupport.h"

#include "Avion.h"
#include "Trackball.h"
#include "UtilGL.h"
#include "Camera.h"
#include <QtEvents>


/**
  @class GLView

  The widget to draw with OpenGL.
  */
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


  // TP :

private:
  bool _mouseLeftButton; //! true when mouse left is pressed
  bool _mouseRightButton; //! true when mouse right is pressed

  float _angle;
  prog3d::UtilGL *_utilGL;

  // TODO  
  bool _pressLeft;    // appui sur la touche 'q'
  bool _pressRight;   // appui sur la touche 'd'
  bool _pressUp;      // appui sur la touche 'z'
  bool _pressDown;    // appui sur la touche 's'
  bool _pressRollLeft;      //
  bool _pressRollRight;    //
  bool _pressAccelerate;
  bool _pressDecelerate;
  Avion _avion;


  prog3d::Trackball _trackball; // contrôle du placement de la scène à la souris

  /** Variables globales pour l'application */
  double _kIncrement;
  double _kInterpolate;
  prog3d::Camera _camera,_cam1,_cam2;



signals:

public slots:
  //! called every 20ms by the MainWindow
  void updateData();

};

#endif // GLVIEW_H
