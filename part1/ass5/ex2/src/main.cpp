#include <iostream>
#include <QDesktopWidget>
#include <QApplication>
#include <QStyle>
#include "MainWindow.h"

int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.setWindowTitle("My fancy editor");
  mainWindow.show();

  return app.exec();
}
