#include <QtWidgets>
#include <iostream>

#include <gui/PoseWidget.h>

PoseWidget::PoseWidget(QWidget *parent) : QWidget(parent) {
}

PoseWidget::~PoseWidget() {

}

void PoseWidget::setNumberOfPoses(int n) {
	numberOfPoses_ = n;
}

void PoseWidget::setDetectorChoice(int n) {
	chosenDetector_ = n;
	update();
}

void PoseWidget::setDistance(double d) {
	distance_ = d;
	update();
}

int PoseWidget::getNumberOfPoses() const {
	return numberOfPoses_;
}

void PoseWidget::setVolumeSizes(Eigen::AlignedBox3f volume) {
	volumeSizes_ = volume;
}

void PoseWidget::setDetectorSizeX(double sizeX){
	detectorSizeX_ = sizeX;
	update();
}

void PoseWidget::setDetectorSizeY(double sizeY){
	detectorSizeY_ = sizeY;
	update();
}

void PoseWidget::paintEvent(QPaintEvent *event){
	if(!volumeSizes_.isEmpty()){
		QPainter painter(this);
		QPen pen;
		pen.setColor("black");

		int w = width();
		int h = height();
		int size = std::min(w, h);
		float scaleDistance = distance_ * (size*0.01);
		int changeDetectorX = detectorSizeX_ *(size*0.05);
		int dX = size/4 + scaleDistance;
		float penWidth = 3+(detectorSizeY_*3);

		/*draw the volume*/
		QRectF volume(w*0.45, h*0.3, size/4, size/4);
		painter.setBrush(Qt::black);
		painter.drawRect(volume);

		painter.translate(volume.center().x(), volume.center().y());
		/*angle to rotate the source and detector*/
		qreal degree = 0;
		for (int i = 0; i < numberOfPoses_; i++) {
			 if(i+1==chosenDetector_){
				 pen.setColor("blue");			 	//	painter.setPen(Qt::blue);
				 pen.setWidth(3);
				 painter.setPen(pen);
			 }
			if(i>0) degree = -360.0/numberOfPoses_;
			painter.rotate(degree);

			QRectF source(-size/4 - scaleDistance, -5, 12.5, 12.5);
			QPoint s = QPoint(source.topLeft().x()+2.5,source.topLeft().y()+10.5);
			painter.setBrush(Qt::white);
			painter.drawEllipse(source);
			painter.drawText(s, "s");

			if(i+1==chosenDetector_)
				pen.setWidth(penWidth);
			else pen.setWidth(detectorSizeY_*3);
			painter.setPen(pen);
			QLineF detector(dX, -size/4 - changeDetectorX, dX, size/4 + changeDetectorX);
			painter.drawLine(detector);
			pen.setWidth(1);
			pen.setColor("black");
			painter.setPen(pen);
	  }
	}
}
