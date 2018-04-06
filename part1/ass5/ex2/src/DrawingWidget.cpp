#include "DrawingWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include<QMessageBox>
#include<QColor>
#include<QtWidgets>


  DrawingWidget::DrawingWidget(QWidget *parent)
  : QWidget(parent),
    my_penColor(Qt::black),
    my_penSize(1),
    my_zoom(1){
      setBackgroundRole(QPalette::Base);
      setMinimumSize(QSize(1000, 600));
    }

  const QImage& DrawingWidget::image() const{
    return my_image;
  }

  double DrawingWidget::zoom() const{
    return my_zoom;
  }

  QColor DrawingWidget::penColor() const{
    return my_penColor;
  }

  unsigned int DrawingWidget::penSize() const{
    return my_penSize;
  }

  void DrawingWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    resize(my_zoom * my_image.size());
    painter.scale(my_zoom, my_zoom);
    painter.drawImage(0, 0, my_image);
  }

  void DrawingWidget::mousePressEvent(QMouseEvent* event){
    if(image().isNull()){
      QMessageBox messageBox;
      messageBox.critical(0,"UPS!","You need to load an image first!");
      messageBox.setFixedSize(500,200);
    } else {
      pressedPen = true;
      QPoint mapCoordinates = transform.map(event->pos());
      if (!usePen) {
        QPoint pixelAtPosition;
        pixelAtPosition.setX(mapCoordinates.x()/my_zoom);     //make it possible for the colorPicker to take the color of pixels, when zoomIn/Out
        pixelAtPosition.setY(mapCoordinates.y()/my_zoom);
        if(pixelAtPosition.x() < my_image.width() && pixelAtPosition.y() < my_image.height()){    //check color only if inside the image
         QRgb rgb = my_image.pixel(pixelAtPosition);
         const QColor color(rgb);
         emit colorReadout(color);
        }
      } else paintPixel(mapCoordinates);
    }
  }

  void DrawingWidget::mouseMoveEvent(QMouseEvent* event){
    if (usePen && pressedPen) {
      QPoint mapCoordinates = transform.map(event->pos());
      paintPixel(mapCoordinates);
    }
  }

  void DrawingWidget::mouseReleaseEvent(QMouseEvent* event){
      pressedPen = false;
  }

  void DrawingWidget::paintPixel(QPoint mapCoordinates){
    QPoint position;
    position.setX((mapCoordinates.x() - my_penSize)/ my_zoom);
    position.setY((mapCoordinates.y() - my_penSize)/ my_zoom);

    if(position.x() < my_image.width() && position.y() < my_image.height()){  //paint only inside the image
      for (int i = position.x(); i < (position.x() + my_penSize); i++) {
        for (int j = position.y(); j < (position.y() + my_penSize); j++) {
          my_image.setPixel(i, j, my_penColor.rgb());
        }
      }
    }

    update();
  }

  void DrawingWidget::setImage(const QImage& newImage){
    my_image = newImage;
    update();
  }

  void DrawingWidget::setZoom(double zoomFactor){
    my_zoom *= zoomFactor;
    update();
  }

  void DrawingWidget::setPenColor(const QColor& newColor){
    if(newColor.isValid())
      my_penColor = newColor;
  }

  void DrawingWidget::setPenSize(int penSize){
     my_penSize = penSize;
  }

  void DrawingWidget::setUsageOfPen(const bool isPen) {
    usePen = isPen;
  }
