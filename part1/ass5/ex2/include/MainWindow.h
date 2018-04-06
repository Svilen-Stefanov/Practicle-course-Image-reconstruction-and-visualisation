#pragma once

#include<QLabel>
#include<QScrollArea>
#include<QMainWindow>
#include "DrawingWidget.h"


class MainWindow : public QMainWindow {
  Q_OBJECT

    public:
      MainWindow(QWidget *parent = 0);
      ~MainWindow();

     private slots:
      void open();
      bool loadFile(const QString &fileName);
      void save();
      void zoomIn();
      void zoomOut();
      void setPenView();
      void setPenSetup();
      void pickColor();
      void setCursorSetup();

    public slots:
      void updateStatusBar(const QColor& color);

    private:
      DrawingWidget *drawingWidget;
      QLabel *imageLabel;
      QScrollArea *scrollArea;
      QToolBar *toolBar;

      void enableButtons();
      void createMenu();
      void addStatusBar();
      void adjustScroll(QScrollBar *scrollBar, double factor);

      QAction *loadAct;
      QAction *saveAsAct;
      QAction *zoomInAct;
      QAction *zoomOutAct;

      QAction *penAct;
      QAction *pickColorAct;
      QAction *cursorAct;
      QAction *penSizeComboBoxAct;
      QAction *penSetColorAct;
      QAction *exitAct;
};
