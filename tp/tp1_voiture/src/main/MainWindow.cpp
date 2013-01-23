
/**

  @author F. Aubert
  **/
#include "MainWindow.h"
#include <QtGui>

#include "GLView.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) { // base class initialization

  // create a layout and attach it to the widget
  QGridLayout *gridLayout=new QGridLayout();
  this->setLayout(gridLayout);

  // OpenGL context initializations and creation of the glView (see glView.cpp)
  QGLFormat format=QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::StencilBuffer | QGL::AlphaChannel);
  GLView *glView=new GLView(format,this);

  // add widget to the layout
  gridLayout->addWidget(glView);

  // get the focus
  glView->setFocus();

  // call the glView drawing every 20ms (i.e. 50Hz)
  QTimer *timer=new QTimer(this);
  connect(timer,SIGNAL(timeout()),glView,SLOT(updateData())); // call glView->update() (that will call paintGL() )
  timer->start(20);
}


MainWindow::~MainWindow() {
  // no need to delete glView : glView will be deleted when its parent will be deleted
}


// default size of the MainWindow
QSize MainWindow::sizeHint() const {
  return QSize(512,512);
}

// key events
void MainWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key()==Qt::Key_Escape) {
    this->close();
  }
  else {
    QWidget::keyPressEvent(e); // send the event to the parent widget
  }
}

