/**

  @author F. Aubert
  **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "Raytrace.h"
#include "Scene.h"
#include "GLView.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void keyPressEvent(QKeyEvent *e);

  QSize sizeHint() const;
private:
  void loadScene(unsigned int number);

  Raytrace *_raytrace;
  Scene *_scene;
  GLView *_glView;

  int _nbScene;
  int _idScene;

};

#endif // MAINWINDOW_H

