#ifndef RECON_WIDGET_H
#define RECON_WIDGET_H

#include <QLabel>
#include <QtWidgets>
#include <QImage>

#include <math/AcquisitionPoses.h>
#include <math/Volume.h>
#include <gui/SliceWidget.h>

class ReconstructionWidget : public QWidget {

	Q_OBJECT;

public:
	ReconstructionWidget(QWidget* parent = nullptr);
	~ReconstructionWidget();

	//edf file?
private:
	void connectSlots();

	//variables that are needed
	int numIterations_;
	double epsilon_;
	double lambda_;
	bool useNoise_;
	int numNoisePercentage_;
	int resX_;
	int resY_;
	int resZ_;

	//gui stuff
	QGroupBox* tomoRecoGroup();
	QGroupBox* parameterGroup();
	QGroupBox* noiseGroup();

	//tomographic reconstruction - two implementations
	QPushButton* normalReconstruction_;
	QPushButton* regularizedReconstruction_;

	//finding good parameters
	QSpinBox* resXSpinBox_;
	QSpinBox* resYSpinBox_;
	QSpinBox* resZSpinBox_;
	QSpinBox* iterationsSpinBox_;
	QDoubleSpinBox* epsilonSpinBox_;
	QDoubleSpinBox* lambdaSpinBox_;

	//noise
	QCheckBox* noiseCheckbox_;
	QSpinBox* noisePercentageSpinBox_;

	//input
	AcquisitionPoses poses_;
	bool posesDefined_; //poses are filled with legal values

	//output
	SliceWidget* sliceWidget_;

	void solve(bool useRegularizer);

public slots:
	//settings
	void setUseNoise(int b);
	void setPercentage(int p);
	void setIterations(int i);
	void setResX(int x);
	void setResY(int y);
	void setResZ(int z);
	void setEpsilon(double e);
	void setLambda(double l);

	//input from previous stage
	void setAcquisitionPoses(const AcquisitionPoses* const poses);
	//buttons
	void performNormalReconstruction();
	void performRegularizedReconstruction();

signals:
	void sendReconstructedVolume(Volume_ptr vol);
};


#endif