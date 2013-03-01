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
#include "Entity.h"
#include "Shader.h"
#include "Square.h"
#include "Texture.h"

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

  void drawSquare();
  void drawCowLight();
  void drawCowGooch();
  void drawCowCel();
  void drawCowOutline();



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

  double _zCam;

  unsigned int _modeDisplay;

  prog3d::Shader _shaderSimple,_shaderLight,_shaderOutline,_shaderGooch,_shaderCel;
  Square _square;
  Entity _cow;

  prog3d::Texture _tex0,_tex1,_tex2;

  float _nbCellStep;

  GLuint _locationFactor;
  GLuint _locationCelStep;


  prog3d::Trackball _trackball; // contrôle du placement de la scène à la souris


signals:

public slots:
  //! called every 20ms by the MainWindow
  void updateData();

};

#endif // GLVIEW_H

