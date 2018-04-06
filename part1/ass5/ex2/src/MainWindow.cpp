#include<QComboBox>
#include<QToolBar>
#include<QColor>
#include<QScrollBar>
#include<QColorDialog>
#include<QStatusBar>
#include<QAction>
#include<QApplication>
#include<QDesktopWidget>
#include<QStyle>
#include<QGuiApplication>
#include<QMessageBox>
#include<QImageWriter>
#include<QImageReader>
#include<QFileDialog>
#include<QStandardPaths>
#include<QScrollArea>
#include<QGroupBox>
#include"MainWindow.h"

  MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     drawingWidget(new DrawingWidget),
     imageLabel(new QLabel),
     scrollArea(new QScrollArea){

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(drawingWidget);
    scrollArea->setVisible(true);

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    this->setGeometry(    //center
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    );

    setCentralWidget(scrollArea);

    createMenu();

    addStatusBar();
    connect(drawingWidget, &DrawingWidget::colorReadout, this, &MainWindow::updateStatusBar);
  }

  // update scroll bar
  void MainWindow::adjustScroll(QScrollBar *scrollBar, double index) {
     scrollBar->setValue(int(index * scrollBar->value() + ((index- 1)
                      * scrollBar->pageStep() / 2)));
  }

  MainWindow::~MainWindow() {
    delete drawingWidget;
    delete imageLabel;
    delete scrollArea;
  }

  void MainWindow::addStatusBar() {
    statusBar()->addPermanentWidget(imageLabel);
  }

  void MainWindow::updateStatusBar(const QColor& color){
    if (pickColorAct->isChecked()) {
      imageLabel->clear();
      imageLabel->setText(QString("R: %1, G: %2, B: %3").arg(color.red()).arg(color.green()).arg(color.blue()));
    }
  }

  void MainWindow::createMenu(){
    toolBar = new QToolBar(this);

    loadAct = toolBar->addAction(QIcon::fromTheme("document-open"), "&Load");
    loadAct->setShortcut(QKeySequence("Ctrl+L"));
    connect(loadAct, &QAction::triggered, this, &MainWindow::open);
    toolBar->addSeparator();

    saveAsAct = toolBar->addAction(QIcon::fromTheme("document-save"), "&Save As");
    saveAsAct->setEnabled(false);
    saveAsAct->setShortcut(QKeySequence("Ctrl+S"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::save);
    toolBar->addSeparator();

    zoomInAct = toolBar->addAction(QIcon::fromTheme("zoom-in"), "Zoom In");
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    zoomInAct->setShortcut(QKeySequence("Ctrl+Shift+Plus"));
    connect(zoomInAct, &QAction::triggered, this, &MainWindow::zoomIn);
    toolBar->addSeparator();

    zoomOutAct = zoomOutAct = toolBar->addAction(QIcon::fromTheme("zoom-out"), "Zoom Out");
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    zoomInAct->setShortcut(QKeySequence("Ctrl+Shift+Minus"));
    connect(zoomOutAct, &QAction::triggered, this, &MainWindow::zoomOut);
    toolBar->addSeparator();

    setPenSetup();

    exitAct = toolBar->addAction(QIcon::fromTheme("application-exit"), "E&xit");
    connect(exitAct, &QAction::triggered, this, &QApplication::quit);

    addToolBar(Qt::LeftToolBarArea, toolBar);
  }

  void MainWindow::setPenSetup(){
    const QIcon penIcon = QIcon(":/images/pen.png");
    penAct = toolBar->addAction(penIcon, "&Pen"); //image
    penAct->setCheckable(true);
    penAct->setEnabled(false);
    connect(penAct, &QAction::triggered, this, &MainWindow::setPenView);
    toolBar->addSeparator();

    const QIcon pickColorIcon = QIcon(":/images/pick-color-icon.png");
    pickColorAct = toolBar->addAction(pickColorIcon, "&Pick Color");
    pickColorAct->setCheckable(true);
    pickColorAct->setEnabled(false);
    pickColorAct->setVisible(true);
    connect(pickColorAct, &QAction::triggered, this, &MainWindow::pickColor);
    toolBar->addSeparator();

    const QIcon colorPickerIcon = QIcon(":/images/color-picker-icon.png");
    penSetColorAct = toolBar->addAction(colorPickerIcon, "Choose Color");
    connect(penSetColorAct, &QAction::triggered, this, [=](){
      QColor color = QColorDialog::getColor(drawingWidget->penColor(), this);
        drawingWidget->setPenColor(color);
    });
    penSetColorAct->setVisible(true);

      //create a list of possible sizes to chose from
    const QStringList penSizes{"1", "2", "4", "8", "16"};
    QComboBox *chosePenSizes = new QComboBox(this);
    chosePenSizes->addItems(penSizes);
    chosePenSizes->setEditable(true);
    chosePenSizes->setInsertPolicy(QComboBox::InsertAlphabetically);

      //add the Compobox and set the chosen size of pen
    penSizeComboBoxAct = toolBar->addWidget(chosePenSizes);
    connect(chosePenSizes, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int sizeOfPen) {
      drawingWidget->setPenSize(chosePenSizes->itemText(sizeOfPen).toInt());
    });
    penSizeComboBoxAct->setVisible(true);

    const QIcon cursorIcon = QIcon(":/images/cursor-arrow.png");
    cursorAct = toolBar->addAction(cursorIcon, "Cursor arrow");
    cursorAct->setCheckable(true);
    cursorAct->setChecked(false);
    connect(cursorAct, &QAction::triggered, this, &MainWindow::setCursorSetup);
    toolBar->addSeparator();
  }

  /*Set default location and the types of the file
  based on example from QT documentation - ImageViewer Class
  Used for loading and saving images*/
  static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode){
      static bool firstDialog = true;

      if (firstDialog) {
          firstDialog = false;
          const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
          dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
      }

      QStringList mimeTypeFilters;
      const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
          ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
      foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
          mimeTypeFilters.append(mimeTypeName);
      mimeTypeFilters.sort();
      dialog.setMimeTypeFilters(mimeTypeFilters);
      dialog.selectMimeTypeFilter("image/jpeg");
      if (acceptMode == QFileDialog::AcceptSave)
          dialog.setDefaultSuffix("jpg");
  }

   /*Main function for loading a file*/
  void MainWindow::open(){
      QFileDialog dialog(this, tr("Open File"));
      initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

      while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
  }

  /*Check whether the file was successful to load and set default values*/
  bool MainWindow::loadFile(const QString &fileName){
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    drawingWidget->setImage(QImage(reader.read()));
    if (drawingWidget->image().isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }
    drawingWidget->setZoom(1.0);

    enableButtons();
    imageLabel->adjustSize();
    return true;
  }


  void MainWindow::enableButtons(){
    saveAsAct->setEnabled(true);
    scrollArea->setVisible(true);
    // enable toolbar items
    zoomInAct->setEnabled(true);
    zoomOutAct->setEnabled(true);
    penAct->setEnabled(true);
    pickColorAct->setEnabled(true);
  }

  void MainWindow::save(){
    QFileDialog *dialog = new QFileDialog(this, tr("Save File"));
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    //saves the image as .jpg as default
    initializeImageFileDialog(*dialog, QFileDialog::AcceptSave);

    dialog->open();
    connect(dialog, &QFileDialog::finished, [=](int result){
      if(result) {
        QString fileName(dialog->selectedFiles().first());
        drawingWidget->image().save(fileName);
      }
      dialog->deleteLater();
    });
  }

  void MainWindow::zoomIn(){
    drawingWidget->setZoom(1.25);
    adjustScroll(scrollArea->horizontalScrollBar(), 1.25);
    adjustScroll(scrollArea->verticalScrollBar(), 1.25);
  }

  void MainWindow::zoomOut(){
    drawingWidget->setZoom(0.75);
    adjustScroll(scrollArea->horizontalScrollBar(), 0.75);
    adjustScroll(scrollArea->verticalScrollBar(), 0.75);
  }

  void MainWindow::setPenView(){
    cursorAct->setChecked(false);
    pickColorAct->setChecked(false);
    penAct->setChecked(true);
    drawingWidget->setUsageOfPen(true);

    imageLabel->clear();

    drawingWidget->setCursor(Qt::CrossCursor);
    if (drawingWidget->hasMouseTracking())
      drawingWidget->setMouseTracking(false);
  }

  void MainWindow::pickColor(){
     penAct->setChecked(false);
     cursorAct->setChecked(false);
     pickColorAct->setChecked(true);
     drawingWidget->setUsageOfPen(false);

     drawingWidget->setCursor(Qt::CrossCursor);
     drawingWidget->setMouseTracking(true);
  }

  void MainWindow::setCursorSetup(){
    penAct->setChecked(false);
    pickColorAct->setChecked(false);
    cursorAct->setChecked(true);
    drawingWidget->setUsageOfPen(false);

    imageLabel->clear();

    drawingWidget->setCursor(Qt::PointingHandCursor);
    if (drawingWidget->hasMouseTracking())
      drawingWidget->setMouseTracking(false);
  }
