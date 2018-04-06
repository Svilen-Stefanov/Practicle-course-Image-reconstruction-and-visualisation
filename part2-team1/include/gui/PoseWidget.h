#ifndef PAINTER_WIDGET_H
#define PAINTER_WIDGET_H

#include <QtGui>
#include <QtWidgets>
#include <Eigen/Geometry>

class PoseWidget : public QWidget {

	Q_OBJECT;

public:
	PoseWidget(QWidget* parent = nullptr);
	~PoseWidget();

	void paintEvent(QPaintEvent* event) override;
	int getNumberOfPoses() const;
	void setVolumeSizes(Eigen::AlignedBox3f);

private:
	QSize* size_ = new QSize(400,300);	//probably should make the size dependable on the screen size
	/*size of the volume*/
	Eigen::AlignedBox3f volumeSizes_;

	int numberOfPoses_ = 1;
	int chosenDetector_ = 1;
	/*distance from center of volume to detector and source*/
	float distance_ = 1;
	/*size of detector*/
	double detectorSizeX_ = 0.3;
	double detectorSizeY_ = 0.3;

public slots:
	void setNumberOfPoses(int);
	void setDistance(double);
	void setDetectorSizeX(double);
	void setDetectorSizeY(double);
	void setDetectorChoice(int);

};

#endif
