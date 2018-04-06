#pragma once
#include <QWidget>
#include <QColor>
#include <QScrollArea>
#include <QLabel>

class DrawingWidget : public QWidget {
  Q_OBJECT

  public:
    DrawingWidget(QWidget *parent = 0);

    const QImage& image() const;
    double zoom() const;
    QColor penColor() const;
    unsigned int penSize() const;
    void setUsageOfPen(const bool state);
    bool getUsageOfPen();

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

  public slots:
    void setImage(const QImage& newImage);
    void setZoom(double zoomFactor);
    void setPenColor(const QColor& newColor);
    void setPenSize(int penSize);

  signals:
    void colorReadout(const QColor& color);

  private:
    void paintPixel(QPoint);
    QImage my_image;
    double my_zoom;
    int my_penSize;
    QColor my_penColor;
    bool pressedPen;
    bool usePen;
    QTransform transform;

};
