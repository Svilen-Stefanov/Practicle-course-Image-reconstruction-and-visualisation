#include <QtWidgets>
#include <iostream>
#include <sstream>
#include <QtConcurrent>

#include <gui/AcquisitionWidget.h>
#include <gui/RealtimePreviewWidget.h>
#include <gui/PoseWidget.h>
#include <string>

AcquisitionWidget::AcquisitionWidget(QWidget *parent)
: QWidget(parent)
, numberOfSlices_(1)
, radialCount_(24)
, rotationAxis_(RotationAxis::Z)
, zBeginSlice_(0)
, zEndSlice_(1)
, distance_(1)
, resolution_(10, 10)
, size_(0.40, 0.40)
, selectedRadialPosition_(1)
, selectedSlice_(1)
{
	QHBoxLayout* layout = new QHBoxLayout;
	QVBoxLayout* configLayout = new QVBoxLayout;
	QVBoxLayout* simulationLayout = new QVBoxLayout;
    simulateButton_ = new QPushButton("Simulate");
		simulateButton_->setEnabled(false);

    configLayout->addWidget(inputGroup());
    configLayout->addWidget(detectorGroup());
    configLayout->addWidget(poseGroup());
    configLayout->addWidget(simulateButton_);
	configLayout->addStretch(1);

    pose_ = new PoseWidget();
	pose_->setDistance(distance_);
	pose_->setDetectorSizeX(size_.x());
	pose_->setDetectorSizeY(size_.y());
	pose_->setNumberOfPoses(radialCount_);

    realTime_ = new RealtimePreviewWidget();
    simulationLayout->addWidget(pose_, 1);
    simulationLayout->addWidget(poseSliders(), 0);
    simulationLayout->addWidget(realTime_, 1);
    layout->addLayout(configLayout, 0);
    layout->addLayout(simulationLayout, 1);
    setLayout(layout);
    connectSlots();

    anglesInDegrees_ = 360 / radialCount_;
    acquisitionPoses_ = new AcquisitionPoses();
    updateAcquisitionPoses();

}

AcquisitionWidget::~AcquisitionWidget() {
	delete pose_;
	delete simulateButton_;
	delete realTime_;
	delete selectedDetector_;
	delete amountOfPosesLabelName_;
	delete acquisitionPoses_;
}

QGroupBox* AcquisitionWidget::inputGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Input:"));
	QVBoxLayout* layout = new QVBoxLayout();

    QLabel* resolutionInputName = new QLabel("Resolution:",groupBox);
    resolutionInput_ = new QLabel("?",groupBox);
    QLabel* sizeInputName = new QLabel("Size:",groupBox);
    sizeInput_ = new QLabel("?",groupBox);
    layout->addWidget(resolutionInputName);
    layout->addWidget(resolutionInput_);
    layout->addWidget(sizeInputName);
    layout->addWidget(sizeInput_);
    groupBox->setLayout(layout);
	return groupBox;
}

QGroupBox* AcquisitionWidget::detectorGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Detector:"));
	QVBoxLayout* layout1 = new QVBoxLayout();
	QHBoxLayout* layout2 = new QHBoxLayout();
	QHBoxLayout* layout3 = new QHBoxLayout();

 	QLabel* resolutionDetectorName = new QLabel("Resolution:");
 	detectorResX_ = new QSpinBox();
 	detectorResX_->setMinimum(1);
 	detectorResX_->setMaximum(100);
	detectorResX_->setValue(resolution_.x());
 	detectorResY_ = new QSpinBox();
 	detectorResY_->setMinimum(1);
 	detectorResY_->setMaximum(100);
	detectorResY_->setValue(resolution_.y());
 	QLabel* resX = new QLabel("X");
 	layout2->addWidget(detectorResX_);
 	layout2->addWidget(resX);
	QLabel* resY = new QLabel("Y");
 	layout2->addWidget(detectorResY_);
	layout2->addWidget(resY);

 	QLabel* sizeDetectorName = new QLabel("Size:");
 	detectorSizeX_ = new QDoubleSpinBox();
 	detectorSizeX_->setMinimum(0.2);
 	detectorSizeX_->setMaximum(1.0);
	detectorSizeX_->setValue(size_.x());
  	detectorSizeX_->setSingleStep(0.1);
 	detectorSizeY_ = new QDoubleSpinBox();
 	detectorSizeY_->setMinimum(0.2);
 	detectorSizeY_->setMaximum(1.0);
	detectorSizeY_->setValue(size_.y());
  	detectorSizeY_->setSingleStep(0.1);
 	QLabel* sizeX = new QLabel("X");
 	layout3->addWidget(detectorSizeX_);
 	layout3->addWidget(sizeX);
	QLabel* sizeY = new QLabel("Y");
 	layout3->addWidget(detectorSizeY_);
	layout3->addWidget(sizeY);

 	QLabel* detectorDistanceName = new QLabel("Distance to Source:");
 	detectorDistanceSource_ = new QDoubleSpinBox();
 	detectorDistanceSource_->setMinimum(0.5);
 	detectorDistanceSource_->setMaximum(10.0);
	detectorDistanceSource_->setSingleStep(0.1);
	detectorDistanceSource_->setValue(distance_);

 	layout1->addWidget(resolutionDetectorName);
  	layout1->insertLayout(1,layout2);
 	layout1->addWidget(sizeDetectorName);
  	layout1->insertLayout(3,layout3);
  	layout1->addWidget(detectorDistanceName);
  	layout1->addWidget(detectorDistanceSource_);

 	groupBox->setLayout(layout1);
	return groupBox;
}

QGroupBox* AcquisitionWidget::poseGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Poses:"));
  	QVBoxLayout* layout1 = new QVBoxLayout();
 	QHBoxLayout* layoutPose = new QHBoxLayout();
	QHBoxLayout* layout2 = new QHBoxLayout();
	QHBoxLayout* layout3 = new QHBoxLayout();
	QHBoxLayout* layout4 = new QHBoxLayout();
	QHBoxLayout* layout5 = new QHBoxLayout();

	QLabel* amountOfPosesName = new QLabel("Number of Poses:");

	amountOfPoses_ = new QSlider(Qt::Horizontal, this);
	amountOfPoses_->setMinimum(1);
	amountOfPoses_->setMaximum(64);
	amountOfPoses_->setValue(radialCount_);
	amountOfPoses_->setTickInterval(1);
  	amountOfPosesLabelName_ = new QLabel(QString::number(radialCount_));
	layoutPose->addWidget(amountOfPosesName);
	layoutPose->addWidget(amountOfPosesLabelName_);

	QLabel* slicesNumberName = new QLabel("Number of Slices:", this);
	slicesNumber_ = new QSpinBox();
	slicesNumber_->setMinimum(1);
	slicesNumber_->setValue(numberOfSlices_);
	layout2->addWidget(slicesNumberName);
	layout2->addWidget(slicesNumber_);

	QLabel* axisLabel = new QLabel("Rotation axis:", this);
	axisComboBox_ = new QComboBox(this);
	axisComboBox_->addItem("X");
	axisComboBox_->addItem("Y");
	axisComboBox_->addItem("Z");
	axisComboBox_->setCurrentIndex(rotationAxis_);
	layout5->addWidget(axisLabel);
	layout5->addWidget(axisComboBox_);

	QLabel* slicesBeginZName = new QLabel("Translation begin:", this);
	slicesBeginZ_ = new QDoubleSpinBox();
	slicesBeginZ_->setMinimum(-1);
	slicesBeginZ_->setMaximum(1);
	slicesBeginZ_->setDecimals(3);
	slicesBeginZ_->setValue(zBeginSlice_);
	slicesBeginZ_->setSingleStep(0.01);
	layout3->addWidget(slicesBeginZName);
	layout3->addWidget(slicesBeginZ_);

	QLabel* slicesEndZName = new QLabel("Translation end:", this);
	slicesEndZ_ = new QDoubleSpinBox();
	slicesEndZ_->setMinimum(-1);
	slicesEndZ_->setMaximum(1);
	slicesEndZ_->setDecimals(3);
	slicesEndZ_->setValue(zEndSlice_);
	slicesEndZ_->setSingleStep(0.01);
	layout4->addWidget(slicesEndZName);
	layout4->addWidget(slicesEndZ_);

	layout1->insertLayout(2,layoutPose);
	layout1->addWidget(amountOfPoses_);
	layout1->insertLayout(3,layout2);
	layout1->insertLayout(4, layout5);
	layout1->insertLayout(5,layout3);
	layout1->insertLayout(6,layout4);

	groupBox->setLayout(layout1);
	return groupBox;
}

QGroupBox* AcquisitionWidget::poseSliders() {
	QGroupBox* groupBox = new QGroupBox();
	QVBoxLayout* layoutSlides = new QVBoxLayout();
	QHBoxLayout* layoutDetectorChoice = new QHBoxLayout();

	QLabel* selectDetector = new QLabel("Select detector:");
	selectedDetector_ = new QLabel("1");
	selectedDetector_->setAlignment(Qt::AlignLeading);
	detectorChoice_ = new QSlider(Qt::Horizontal);
	detectorChoice_->setMinimum(1);
	detectorChoice_->setMaximum(radialCount_);
	detectorChoice_->setValue(1);
	detectorChoice_->setTickInterval(1);


	QLabel* selectSlice = new QLabel("Select slice:");
	sliceChoice_ = new QSlider(Qt::Horizontal);
	sliceChoice_->setMinimum(1);
	sliceChoice_->setMaximum(numberOfSlices_);
	sliceChoice_->setValue(1);
	sliceChoice_->setTickInterval(1);

	layoutDetectorChoice->addWidget(selectDetector);
	layoutDetectorChoice->addWidget(selectedDetector_);
	layoutSlides->insertLayout(1,layoutDetectorChoice);
	layoutSlides->addWidget(detectorChoice_);
	layoutSlides->addWidget(selectSlice);
	layoutSlides->addWidget(sliceChoice_);

	groupBox->setLayout(layoutSlides);
	return groupBox;
}

void AcquisitionWidget::connectSlots() {
	connect(slicesNumber_,SIGNAL(valueChanged(int)),SLOT(setNumberOfSlices(int)));
	connect(slicesEndZ_, SIGNAL(valueChanged(double)), SLOT(setZEndSlice(double)));
	connect(slicesBeginZ_, SIGNAL(valueChanged(double)), SLOT(setZBeginSlice(double)));
	connect(amountOfPoses_,SIGNAL(valueChanged(int)),SLOT(setNumberOfPoses(int)));
	connect(amountOfPoses_,SIGNAL(valueChanged(int)),pose_,SLOT(setNumberOfPoses(int)));
	connect(detectorResX_,SIGNAL(valueChanged(int)),SLOT(setDetectorResX(int)));
	connect(detectorResY_,SIGNAL(valueChanged(int)),SLOT(setDetectorResY(int)));
	connect(detectorSizeY_,SIGNAL(valueChanged(double)),SLOT(setDetectorSizeY(double)));
	connect(detectorSizeY_,SIGNAL(valueChanged(double)),pose_,SLOT(setDetectorSizeY(double)));//
	connect(detectorSizeX_,SIGNAL(valueChanged(double)),SLOT(setDetectorSizeX(double)));
	connect(detectorSizeX_,SIGNAL(valueChanged(double)),pose_,SLOT(setDetectorSizeX(double)));//
	connect(detectorDistanceSource_,SIGNAL(valueChanged(double)),SLOT(setDistance(double)));
	connect(detectorDistanceSource_,SIGNAL(valueChanged(double)),pose_,SLOT(setDistance(double)));
	connect(simulateButton_,SIGNAL(clicked()),SLOT(simulateAcquisition()));
	connect(amountOfPoses_,SIGNAL(valueChanged(int)),SLOT(setPoseLabel(int)));
	connect(axisComboBox_, SIGNAL(currentIndexChanged(int)), SLOT(setRotationAxis(int)));
	connect(sliceChoice_, SIGNAL(valueChanged(int)), SLOT(setSliceChoice(int)));
	connect(detectorChoice_, SIGNAL(valueChanged(int)), SLOT(setDetectorChoice(int)));
	connect(detectorChoice_,SIGNAL(valueChanged(int)),SLOT(setDetectorChoiceLabel(int)));
	connect(detectorChoice_, SIGNAL(valueChanged(int)), pose_, SLOT(setDetectorChoice(int)));
	qRegisterMetaType<AcquisitionPose>();
	connect(this, SIGNAL(sendSelection(const AcquisitionPose&, int, int)), realTime_, SLOT(setSelectedDetector(const AcquisitionPose&)));
}
void AcquisitionWidget::setPoseLabel(int i) {
    amountOfPosesLabelName_->setText(QString::number(i));
}

void AcquisitionWidget::setDetectorChoice(int i)
{
	selectedRadialPosition_ = i;
	emit(sendDetectorChosen(i));
	triggerSelectionChange();
}

void AcquisitionWidget::setDetectorChoiceLabel(int i) {
    selectedDetector_->setText(QString::number(i));
}

void AcquisitionWidget::setSliceChoice(int i)
{
	selectedSlice_ = i;
	triggerSelectionChange();
}

void AcquisitionWidget::setDetectorResX(int i) {
	if (i <= 0) {
		throw std::invalid_argument("invalid resolution");
	}

	resolution_[0] = i;
	updateGlobalPoseSettings();
}

void AcquisitionWidget::setDetectorResY(int i) {
	if (i <= 0) {
        throw std::invalid_argument("invalid resoltuion");
	}
	resolution_[1] = i;
	updateGlobalPoseSettings();
}

void AcquisitionWidget::setDetectorSizeX(double f) {
    if (f <= 0) {
        throw std::invalid_argument("invalid size");
    }
    size_[0] = f;
	updateGlobalPoseSettings();
		emit(sendDetectorSizeX(f));
}

void AcquisitionWidget::setDetectorSizeY(double f) {
    if (f <= 0) {
        throw std::invalid_argument("invalid size");
    }
    size_[1] = f;
	updateGlobalPoseSettings();
		emit(sendDetectorSizeY(f));
}

void AcquisitionWidget::setNumberOfPoses(int i){
	if (i < 1) {
		throw std::invalid_argument("invalid number of poses");
	}
	radialCount_ = i;
	// TO DO: anglesInDegrees_ should be calculated
	anglesInDegrees_ = 360 / i;
	updateAcquisitionPoses();
	emit(sendNumberOfPoses(radialCount_)); //might not need this
	emit(sendAnglesInDegrees(anglesInDegrees_)); // might not need this
	detectorChoice_->setMaximum(i);
}

void AcquisitionWidget::setRotationAxis(int axis)
{
	rotationAxis_ = (RotationAxis)axis;
	updateAcquisitionPoses();
}

void AcquisitionWidget::updateAcquisitionPoses() {
	acquisitionPoses_->getPoses().clear();
    float degreeValue;
    Eigen::Quaternionf rotation;
    Eigen::AngleAxisf angleAxis;
	Eigen::Vector3f center(0, 0, 0);
	Eigen::Vector3f axis;
	switch (rotationAxis_)
	{
	case X: axis = Eigen::Vector3f(1, 0, 0); break;
	case Y: axis = Eigen::Vector3f(0, 1, 0); break;
	default: axis = Eigen::Vector3f(0, 0, 1); break;
	}
	if (volume_ != nullptr) center = volume_->getBoundingBox().center();
	for (int j = 0; j < numberOfSlices_; ++j) {
		double z = zBeginSlice_ + ((j == 0) ? 0 : (j*(zEndSlice_ - zBeginSlice_) / (double)(numberOfSlices_ - 1)));
		for (int i = 0; i < radialCount_; i++) {
			Detector d = getDetector();
			degreeValue = (((i)*anglesInDegrees_) / 180.0) * PI;
			angleAxis = Eigen::AngleAxisf(degreeValue, axis);
			rotation = angleAxis;
			d.setRotation(rotation);
			d.setCenter(center + (float)z * axis);
			AcquisitionPose poseToAdd(d);
			acquisitionPoses_->getPoses().push_back(poseToAdd);
		}
	}
	detectorChoice_->setMaximum(radialCount_);
	detectorChoice_->setValue(std::min(radialCount_, selectedRadialPosition_));
	sliceChoice_->setMaximum(numberOfSlices_);
	sliceChoice_->setValue(std::min(numberOfSlices_, selectedSlice_));
	triggerSelectionChange();
	//qDebug() << "acquisition poses updated";
}


Detector AcquisitionWidget::getDetector() {
	Detector d;
    d.setSize(size_);
    d.setResolution(resolution_);
	d.setDistance(distance_);
	return d;
}

void AcquisitionWidget::triggerSelectionChange()
{
	if (volume_ == nullptr) return;
	size_t idx = std::min(acquisitionPoses_->getPoses().size(), (size_t)selectedRadialPosition_-1 + (selectedSlice_-1)*radialCount_);
	emit sendSelection(acquisitionPoses_->getPoses()[idx], selectedRadialPosition_, selectedSlice_);
}

void AcquisitionWidget::updateAcqusitionPosesBoundingBox() {
	Eigen::AlignedBox3f box = volume_->getBoundingBox();
    if (box.isEmpty()) {
        throw std::logic_error("cannot get bounding box from file");
    }
    acquisitionPoses_->setBoundingBox(box);

	std::stringstream sizeStr;
	sizeStr << "(" << box.min().x() << "," << box.min().y() << "," << box.min().z() << ") x ("
		<< box.max().x() << "," << box.max().y() << "," << box.max().z() << ")";
	sizeInput_->setText(QString::fromStdString(sizeStr.str()));

    Eigen::Vector3i numv = volume_->getNumVoxels();
    std::string r = std::to_string(numv[0]) +" x " + std::to_string(numv[1]) + " x " + std::to_string(numv[2]);
    resolutionInput_->setText(QString::fromStdString(r));
}

void AcquisitionWidget::updateGlobalPoseSettings()
{
	for (AcquisitionPose& p : acquisitionPoses_->getPoses())
	{
		p.getDetector().setSize(size_);
		p.getDetector().setResolution(resolution_);
		p.getDetector().setDistance(distance_);
	}
	//qDebug() << "global detector settings updated";
	triggerSelectionChange();
}

void AcquisitionWidget::setDistance(double f) {
	if (f <= 0) {
		throw std::invalid_argument("invalid size");
	}
	distance_ = f;
	updateGlobalPoseSettings();
	emit(sendPoseDistance(distance_));
}

void AcquisitionWidget::setNumberOfSlices(int i) {
	if (i < 1) {
		throw std::invalid_argument("invalid number of slices");
	}
	numberOfSlices_ = i;
	sliceChoice_->setMaximum(i);
	updateAcquisitionPoses();
}

void AcquisitionWidget::setZEndSlice(double i)
{
	zEndSlice_ = i;
	updateAcquisitionPoses();
}

void AcquisitionWidget::setZBeginSlice(double i)
{
	zBeginSlice_ = i;
	updateAcquisitionPoses();
}

void AcquisitionWidget::simulateAcquisition() {
	qDebug() << "Simulate Acquisition";

#if 0
	//serial version
	for (AcquisitionPose& p : acquisitionPoses_->getPoses())
	{
		p.compute(volume_);
	}
#else
	//parallel version
	QProgressDialog dialog;
	dialog.setLabelText(QString("Simulate using %1 thread(s)...").arg(QThread::idealThreadCount()));
	dialog.setWindowModality(Qt::WindowModal);
	QFutureWatcher<void> futureWatcher;
	connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
	connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
	connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)), &dialog, SLOT(setRange(int, int)));
	connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
	futureWatcher.setFuture(QtConcurrent::map(acquisitionPoses_->getPoses(), [this](AcquisitionPose& pose) {pose.compute(volume_); }));
	dialog.exec();
	futureWatcher.waitForFinished();
	if (futureWatcher.future().isCanceled())
	{
		qDebug() << "Cancelled";
		return;
	}

#endif

	//emit signal
	qDebug() << "all poses are computed and will be sent to the next stage";
	emit sendAcquisitionPoses(acquisitionPoses_);
}

void AcquisitionWidget::setVolume(Volume_ptr volume){
	volume_ = volume;
	pose_->setVolumeSizes(volume_->getBoundingBox());
	realTime_->setVolume(volume);
	simulateButton_->setEnabled(true);
	updateAcqusitionPosesBoundingBox();
	updateAcquisitionPoses();
	triggerSelectionChange();
}
