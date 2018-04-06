#include <QtWidgets>
#include <iostream>
#include <sstream>
#include <QtDebug>

#include <gui/ReconstructionWidget.h>
#include <math/Reconstruction.h>

ReconstructionWidget::ReconstructionWidget(QWidget *parent) 
	: QWidget(parent)
	, posesDefined_(false)
	, numIterations_(100)
	, numNoisePercentage_(5)
	, useNoise_(true)
	, epsilon_(0.00005)
	, lambda_(20)
	, resX_(10)
	, resY_(10)
	, resZ_(10)
{
	QHBoxLayout* layoutMain = new QHBoxLayout;
	QVBoxLayout* groupLayout = new QVBoxLayout;

	groupLayout->addWidget(parameterGroup());
	groupLayout->addWidget(noiseGroup());
	groupLayout->addWidget(tomoRecoGroup());
	groupLayout->addStretch(1);
	layoutMain->addLayout(groupLayout,0);

	sliceWidget_ = new SliceWidget(this);
	layoutMain->addWidget(sliceWidget_, 1);

	setLayout(layoutMain);

	connectSlots();
}

ReconstructionWidget::~ReconstructionWidget() {
	
}

void ReconstructionWidget::connectSlots() {
	//connect slots here
	connect(iterationsSpinBox_,SIGNAL(valueChanged(int)),SLOT(setIterations(int)));
	connect(noisePercentageSpinBox_,SIGNAL(valueChanged(int)),SLOT(setPercentage(int)));
	connect(noiseCheckbox_, SIGNAL(stateChanged(int)), SLOT(setUseNoise(int)));
	connect(resXSpinBox_, SIGNAL(valueChanged(int)), SLOT(setResX(int)));
	connect(resYSpinBox_, SIGNAL(valueChanged(int)), SLOT(setResY(int)));
	connect(resZSpinBox_, SIGNAL(valueChanged(int)), SLOT(setResZ(int)));
	connect(epsilonSpinBox_, SIGNAL(valueChanged(double)), SLOT(setEpsilon(double)));
	connect(lambdaSpinBox_, SIGNAL(valueChanged(double)), SLOT(setLambda(double)));

	connect(normalReconstruction_, SIGNAL(clicked()), this, SLOT(performNormalReconstruction()));
	connect(regularizedReconstruction_, SIGNAL(clicked()), this, SLOT(performRegularizedReconstruction()));
	connect(this, SIGNAL(sendReconstructedVolume(Volume_ptr)), sliceWidget_, SLOT(setVolume(Volume_ptr)));
}

void ReconstructionWidget::setUseNoise(int b)
{
	useNoise_ = (b==Qt::Checked);
}

void ReconstructionWidget::setPercentage(int i) {
	if (i < 0) {
		throw std::invalid_argument("Cannot set percentage of noise to be negative");
	}
	numNoisePercentage_ = i;
}

void ReconstructionWidget::setIterations(int i) {
	if (i < 0) {
		throw std::invalid_argument("Cannot set iterations to be less than 0");
	}
	numIterations_ = i;
}

void ReconstructionWidget::setResX(int x)
{
	resX_ = x;
}

void ReconstructionWidget::setResY(int y)
{
	resY_ = y;
}

void ReconstructionWidget::setResZ(int z)
{
	resZ_ = z;
}

void ReconstructionWidget::setEpsilon(double e)
{
	epsilon_ = e;
}

void ReconstructionWidget::setLambda(double l)
{
	lambda_ = l;
}

void ReconstructionWidget::setAcquisitionPoses(const AcquisitionPoses* const poses)
{
	poses_ = *poses; //copy them
	posesDefined_ = true;
	qDebug() << "acqusition poses received";
}

void ReconstructionWidget::solve(bool useRegularizer)
{
	if (!posesDefined_) {
		qDebug() << "no poses defined yet";
		return;
	}

	//setup solver version
	Reconstruction r;
	Volume_ptr vol = std::make_shared<Volume>(
		poses_.getBoundingBox().min(),
		poses_.getBoundingBox().max(),
		poses_.getBoundingBox().diagonal().array() / Eigen::Vector3f(resX_, resY_, resZ_).array(),
		Eigen::Vector3i(resX_, resY_, resZ_));
	r.setVolume(vol);
	r.setAcquisitionPoses(poses_);
	r.setEpsilon(epsilon_);
	r.setLambda(lambda_);
	r.setNumIterations(numIterations_);
	if (useNoise_)
	{
		r.setNoise(numNoisePercentage_ / 100.0f);
	} else
	{
		r.setNoise(0);
	}
	r.setUseRegularizer(useRegularizer);

	//solve with a progress bar
	QProgressDialog progressDialog("Reconstruct Volume", "Abort", 0, 100, this);
	progressDialog.setWindowModality(Qt::WindowModal);
	progressDialog.setMinimumDuration(1000);
	r.solve([&](const std::string& msg, float progress)
	{
		progressDialog.setLabelText(QString::fromStdString("Reconstruct Volume:\n" + msg));
		progressDialog.setValue(progress * 100);
		return !progressDialog.wasCanceled();
	});
	progressDialog.setValue(100);

	emit sendReconstructedVolume(vol);
}

void ReconstructionWidget::performNormalReconstruction()
{
	solve(false);
}

void ReconstructionWidget::performRegularizedReconstruction()
{
	solve(true);
}

QGroupBox* ReconstructionWidget::tomoRecoGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Tomographic Reconstruction"));
	QVBoxLayout* layoutMain = new QVBoxLayout();

	normalReconstruction_ = new QPushButton("Normal Reconstruction");
	regularizedReconstruction_ = new QPushButton("Regularized Reconstruction");

	layoutMain->addWidget(normalReconstruction_);
	layoutMain->addWidget(regularizedReconstruction_);

    groupBox->setLayout(layoutMain);
	return groupBox;
}

QGroupBox* ReconstructionWidget::parameterGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Parameter Setup"));
	QVBoxLayout* layoutMain = new QVBoxLayout();

	QLabel* resolutionLabel = new QLabel("Resolution:");
	resXSpinBox_ = new QSpinBox(this);
	resXSpinBox_->setMinimum(1);
	resXSpinBox_->setMaximum(100);
	resXSpinBox_->setValue(resX_);
	resYSpinBox_ = new QSpinBox(this);
	resYSpinBox_->setMinimum(1);
	resYSpinBox_->setMaximum(100);
	resYSpinBox_->setValue(resY_);
	resZSpinBox_ = new QSpinBox(this);
	resZSpinBox_->setMinimum(1);
	resZSpinBox_->setMaximum(100);
	resZSpinBox_->setValue(resZ_);
	QHBoxLayout* resLayout = new QHBoxLayout();
	resLayout->addWidget(new QLabel("x:"));
	resLayout->addWidget(resXSpinBox_, 1);
	resLayout->addWidget(new QLabel(" y:"));
	resLayout->addWidget(resYSpinBox_, 1);
	resLayout->addWidget(new QLabel(" z:"));
	resLayout->addWidget(resZSpinBox_, 1);
    
    QLabel* iterationsLabel = new QLabel("Number of Iterations:");
	iterationsSpinBox_ = new QSpinBox();
	iterationsSpinBox_->setMaximum(1000);
	iterationsSpinBox_->setMinimum(0);
	iterationsSpinBox_->setValue(numIterations_);
    
    QLabel* epislonLabel = new QLabel("Epsilon:");
	epsilonSpinBox_ = new QDoubleSpinBox();
	epsilonSpinBox_->setMinimum(0.00001);
	epsilonSpinBox_->setMaximum(1);
	epsilonSpinBox_->setSingleStep(0.00001);
	epsilonSpinBox_->setDecimals(5);
	epsilonSpinBox_->setValue(epsilon_);

	QLabel* lambdaLabel = new QLabel("Lambda:");
	lambdaSpinBox_ = new QDoubleSpinBox();
	lambdaSpinBox_->setMinimum(0);
	lambdaSpinBox_->setMaximum(1000);
	lambdaSpinBox_->setValue(lambda_);

	layoutMain->addWidget(resolutionLabel);
	layoutMain->addLayout(resLayout);
	layoutMain->addWidget(iterationsLabel);
	layoutMain->addWidget(iterationsSpinBox_);
    layoutMain->addWidget(epislonLabel);
	layoutMain->addWidget(epsilonSpinBox_);
	layoutMain->addWidget(lambdaLabel);
	layoutMain->addWidget(lambdaSpinBox_);
    
    groupBox->setLayout(layoutMain);
	return groupBox;
}

QGroupBox* ReconstructionWidget::noiseGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Noise Setup"));
	QVBoxLayout* layoutMain = new QVBoxLayout();
	QHBoxLayout* layoutNoisePercentage = new QHBoxLayout();

	noiseCheckbox_ = new QCheckBox("Use Noise", this);
	noiseCheckbox_->setCheckState(useNoise_ ? Qt::Checked : Qt::Unchecked);
	noisePercentageSpinBox_ = new QSpinBox();
	QLabel* noiseLabel = new QLabel("Noise percentage:");
	noisePercentageSpinBox_->setMinimum(0);
	noisePercentageSpinBox_->setMaximum(100);
	noisePercentageSpinBox_->setValue(numNoisePercentage_);
	layoutNoisePercentage->addWidget(noiseLabel);
	layoutNoisePercentage->addWidget(noisePercentageSpinBox_);

	layoutMain->addWidget(noiseCheckbox_);
	layoutMain->addLayout(layoutNoisePercentage);

	groupBox->setLayout(layoutMain);
	return groupBox;
}