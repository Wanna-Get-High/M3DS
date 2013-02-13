
/**

  @author F. Aubert
  **/

#include <QtGui/QApplication>
#include <iostream>
#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    char c[1000];
    getcwd(c,900);
    cout << c << endl;
  // initialize the Qt application
  QApplication application(argc, argv);

  // create and show the main window
  MainWindow *mainWindow=new MainWindow(NULL);
  mainWindow->show();

  // run the qt application loop (events dispatcher)
  int finished = application.exec();

  // application is finished => clean up code
  delete mainWindow;
  return finished;
}
