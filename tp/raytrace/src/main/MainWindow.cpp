
/**

  @author F. Aubert
  **/
#include "MainWindow.h"

#include "GLView.h"
#include "RaytraceView.h"

#include <QtGui>

#include <string>
#include <sstream>
#include "mediaDirectory.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) { // base class initialization

    _raytrace=NULL;
    _idScene=0;
    _scene=NULL;
  // create a layout and attach it to the widget
  QGridLayout *gridLayout=new QGridLayout();
  this->setLayout(gridLayout);

  // OpenGL context initializations and creation of the glView (see glView.cpp)
  QGLFormat format=QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::StencilBuffer | QGL::AlphaChannel);
  _glView=new GLView(format,this);
  _glView->resize(512,512);

  RaytraceView *raytraceView=new RaytraceView(this);
  raytraceView->resize(512,512);

  _raytrace=new Raytrace(512,512);
  raytraceView->raytrace(_raytrace);


  // add widget to the layout
  gridLayout->addWidget(_glView);
  gridLayout->addWidget(raytraceView,0,1);

  // get the focus
  _glView->setFocus();

  // call the glView drawing every 20ms (i.e. 50Hz)
  QTimer *timer1=new QTimer(this);
  QTimer *timer2=new QTimer(this);
  connect(timer1,SIGNAL(timeout()),_glView,SLOT(updateData())); // call glView->update() (that will call paintGL() )
  connect(timer2,SIGNAL(timeout()),raytraceView,SLOT(repaint()));
  timer1->start(20);
  timer2->start(100);

  _glView->initData();
  loadScene(0);
  _glView->scene(_scene);



}

void MainWindow::loadScene(unsigned int number) {
    if (_scene) delete _scene;
    _scene=new Scene();
    string name="scene";
    ostringstream nb;
    nb << number;
    name+=nb.str();

    name+=".csg";
    cout << "Loading csg : " << "\"" << name << "\"" << endl;
    _scene->read(name);
    _scene->prepareCsg();
    _scene->camera(_glView->camera());
    _scene->trackball(_glView->trackball());
    _raytrace->scene(_scene);
    _glView->scene(_scene);
    _raytrace->start();
}



MainWindow::~MainWindow() {
  // no need to delete glView : glView will be deleted when its parent will be deleted
}


// default size of the MainWindow
QSize MainWindow::sizeHint() const {
  return QSize(1024,512);
}

// key events
void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch(e->key()){
    case Qt::Key_Escape:
      _raytrace->close();
      this->close();
      break;
    case Qt::Key_Space:
      _raytrace->close();
      loadScene(_idScene);
      _raytrace->start();
      break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
      _raytrace->close();
      _idScene=e->key()-Qt::Key_0;

      loadScene(_idScene);

      _raytrace->start();

      break;
    case 'g':
        //_raytrace->switchGrid();
      break;

    default:
      QWidget::keyPressEvent(e); // send the event to the parent widget
    }
}


