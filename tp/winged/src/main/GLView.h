/**
*
*  @author F. Aubert
*  @file GLView
*
*/



#ifndef GLVIEW_H
#define GLVIEW_H

#include "glsupport.h"
#include <QGLWidget>

#include "Trackball.h"
#include "UtilGL.h"
#include "Texture.h"
#include "Winged.h"
#include "Camera.h"

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

  void drawWinged();

  bool _mouseLeftButton; //! true when mouse left is pressed
  bool _mouseRightButton; //! true when mouse right is pressed

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


  prog3d::Trackball _trackball; // contrôle du placement de la scène à la souris


  unsigned int _nbObject;
  Winged *_wingedObject;

  bool _visuPoint;
  bool _visuBoundary;
  unsigned int _visuHide;
  bool _computeVertexNormal;
  bool _drawLineCatmull;

  std::vector<WEdge *> _boundary;

  unsigned int _active;

  prog3d::Texture _ground;
  prog3d::Camera _camera;


signals:

public slots:
  //! called every 20ms by the MainWindow
  void updateData();

};

#endif // GLVIEW_H

