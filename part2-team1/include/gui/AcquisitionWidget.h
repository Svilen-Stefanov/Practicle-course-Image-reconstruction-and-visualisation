#ifndef ACQ_WIDGET_H
#define ACQ_WIDGET_H

#include <QLabel>
#include <QtWidgets>
#include <QImage>
#include <math/Volume.h>
#include <math/AcquisitionPose.h>
#include <math/AcquisitionPoses.h>
#include <math/Detector.h>
#include <gui/RealtimePreviewWidget.h>
#include <gui/PoseWidget.h>


class AcquisitionWidget : public QWidget {

	Q_OBJECT;

public:
	AcquisitionWidget(QWidget* parent = nullptr);
	~AcquisitionWidget();

private:
	//acquisition widget will hold the volume, as well as the poses
	//the poses get updated whenever a setting in the widget is changed
	//for now they are created locally, but since we may pass them around the widget
	//it might make more sense to make a shared pointer?
	Volume_ptr volume_;
	AcquisitionPoses* acquisitionPoses_;

	/**
	 * The rotation axis of the detectors
	 */
	enum RotationAxis 
	{
		X = 0,
		Y,
		Z
	};
	int radialCount_;
	float anglesInDegrees_;
	RotationAxis rotationAxis_;
	const double PI = 3.14159265358979323846;
	QLabel* amountOfPosesLabelName_;
	QLabel* selectedDetector_;

	//for slices configuration
	int numberOfSlices_; //how many slices we will take photos from
	double zBeginSlice_; // where in the z-plane the slices should start - default to
	double zEndSlice_; //where in the z-plane the slices should end
	float distance_; //distance of source to detector

	//for detector configration
	Eigen::Vector2i resolution_; //the resolution of the detector
	Eigen::Vector2f size_;   //the size of the detector

	//for detector selection
	int selectedRadialPosition_;
	int selectedSlice_;

	//for the UI, and elements that have to be declared here to be connected via signals/slots
	QGroupBox* inputGroup();
  	QGroupBox* detectorGroup();
  	QGroupBox* poseGroup();
	QGroupBox* poseSliders();
  	QDoubleSpinBox* slicesEndZ_;
  	QDoubleSpinBox* slicesBeginZ_;
  	QSpinBox* slicesNumber_;
  	QSlider* amountOfPoses_;
	QComboBox* axisComboBox_;
	QSlider* detectorChoice_;
	QSlider* sliceChoice_;
  	QDoubleSpinBox* detectorDistanceSource_;
  	QDoubleSpinBox* detectorSizeY_;
  	QDoubleSpinBox* detectorSizeX_;
  	QSpinBox* detectorResY_;
  	QSpinBox* detectorResX_;

  //for the input group:
  QLabel* resolutionInput_;
  QLabel* sizeInput_;

  //two other widgets:
  PoseWidget* pose_;
  RealtimePreviewWidget* realTime_;

  //simulate button
  QPushButton* simulateButton_;

  void connectSlots();
  void updateAcquisitionPoses();
  void updateAcqusitionPosesBoundingBox();
  void updateGlobalPoseSettings();
  void updateAcquisitionPosesDetector(); //todo: this method is not implemented!
  Detector getDetector();

  void triggerSelectionChange();

public slots:
	void setVolume(Volume_ptr);
	void setDistance(double);
	void setNumberOfSlices(int);
	void setZEndSlice(double);
	void setZBeginSlice(double);
	void setNumberOfPoses(int);
	void setRotationAxis(int);
	void setDetectorResX(int);
	void setDetectorResY(int);
	void setDetectorSizeX(double);
	void setDetectorSizeY(double);
	void simulateAcquisition();
	void setPoseLabel(int);
	void setDetectorChoice(int);
	void setDetectorChoiceLabel(int);
	void setSliceChoice(int);

signals:
	void sendPoseDistance(float); //to send to other class
	void sendNumberOfPoses(int); //to send to other class
	void sendDetectorChosen(int);
	void sendAnglesInDegrees(float); //to send to other class
	void sendDetectorSizeX(double);
	void sendDetectorSizeY(double);

	void sendSelection(const AcquisitionPose& pose, int radialPos, int slice);

	//output to next stage
	void sendAcquisitionPoses(const AcquisitionPoses* const poses);
};

#endif
