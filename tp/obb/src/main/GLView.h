/**
*
*  @author F. Aubert
*  @file GLView
*
*/



#ifndef GLVIEW_H
#define GLVIEW_H

#include "glsupport.h"
#include "BoxList.h"
#include "EngineBox.h"

#include "UtilGL.h"

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
  double _frustumLeft,_frustumRight,_frustumBottom,_frustumTop,_frustumNear,_frustumFar;
  double _viewportX,_viewportY,_viewportWidth,_viewportHeight;

  bool _mouseLeftButton; //! true when mouse left is pressed
  bool _mouseRightButton; //! true when mouse right is pressed


  // TODO  
  bool _pressLeft;    // appui sur la touche 'q'
  bool _pressRight;   // appui sur la touche 'd'
  bool _pressForward;      // appui sur la touche 'z'
  bool _pressBackward;    // appui sur la touche 's'
  bool _pressRollLeft;      //
  bool _pressRollRight;    //
  bool _pressAccelerate;
  bool _pressDecelerate;



  prog3d::Vector3 toWorld(const prog3d::Vector2 &p);


  bool _applyForce;
  prog3d::Vector3 _pointForce;
  BoxList _boxList;
  EngineBox _engineBox;


signals:

public slots:
  //! called every 20ms by the MainWindow
  void updateData();

};

#endif // GLVIEW_H

