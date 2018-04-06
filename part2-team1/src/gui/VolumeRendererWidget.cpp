#include <gui/VolumeRendererWidget.h>

#include <QDebug>
#include <iostream>
#include <random>

class TestTransferFunction : public ITransferFunction
{
private:
	const Eigen::Vector4f start_;
	const Eigen::Vector4f end_;
public:
	TestTransferFunction(const Eigen::Vector4f& start, const Eigen::Vector4f& end)
		: start_(start), end_(end)
	{
		qDebug() << "New transfer function: start=(" << start.x() << "," << start.y() << "," << start.z() << "," << start.w()
			<< "), end=(" << end.x() << "," << end.y() << "," << end.z() << "," << end.w() << ")";
	}
	virtual ~TestTransferFunction() {}
	Eigen::Vector4f classify(float density) const override
	{
		return start_ + density * (end_ - start_);
	}
	static TestTransferFunction* createDefault()
	{
		return new TestTransferFunction(Eigen::Vector4f(1, 1, 1, 0), Eigen::Vector4f(1, 1, 1, 1));
	}
	static TestTransferFunction* createRandom()
	{
		static std::default_random_engine dfe;
		static std::uniform_real_distribution<float> dist(0, 1);
		return new TestTransferFunction(
			Eigen::Vector4f(dist(dfe), dist(dfe), dist(dfe), 0),
			Eigen::Vector4f(dist(dfe), dist(dfe), dist(dfe), 1));
	}
};

VolumeRendererWidget::VolumeRendererWidget(QWidget* parent)
	: transferFunc_(nullptr)
	, testTransferFunction_(nullptr)
	, renderer_(new VolumeRenderer())
	, fixSliceOrientation_(false)
	, sliceOrientation_(0, 0, 1)
	, sliceDistance_(0)
{
	setMinimumWidth(400);
	setCamera(Camera::createTestCamera(Eigen::Vector2i(1,1)));
	testTransferFunction_ = TestTransferFunction::createDefault();
	setTransferFunction(testTransferFunction_);

	//connect renderer
	connect(renderer_, &VolumeRenderer::sendImageChanged, this, &VolumeRendererWidget::onImageChanged, Qt::QueuedConnection);
}

VolumeRendererWidget::~VolumeRendererWidget()
{
	delete renderer_;
	delete testTransferFunction_;
}

void VolumeRendererWidget::setupGui(QGroupBox* box)
{
	QVBoxLayout* layoutMain = new QVBoxLayout();
	layoutMain->setContentsMargins(3, 3, 3, 3);
	layoutMain->setSpacing(3);

	showOutlineCheckBox_ = new QCheckBox("Show outline");
	showOutlineCheckBox_->setChecked(renderer_->getSettings().showBoundingBox_);
	connect(showOutlineCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(setShowOutline(int)));
	layoutMain->addWidget(showOutlineCheckBox_, 1);

	QLabel* stepSizeLabel = new QLabel("Step size:  ");
	stepSizeSpinner_ = new QDoubleSpinBox();
	stepSizeSpinner_->setMinimum(0.0001);
	stepSizeSpinner_->setMaximum(1);
	stepSizeSpinner_->setDecimals(4);
	stepSizeSpinner_->setSingleStep(0.001);
	stepSizeSpinner_->setValue(renderer_->getSettings().stepSize_);
	connect(stepSizeSpinner_, SIGNAL(valueChanged(double)), this, SLOT(setStepSize(double)));
	QHBoxLayout* layout1 = new QHBoxLayout();
	layout1->addWidget(stepSizeLabel);
	layout1->addWidget(stepSizeSpinner_, 1);
	layoutMain->addLayout(layout1);

	QLabel* densityScaleLabel = new QLabel("Density scale:  ");
	densityScaleSpinner_ = new QDoubleSpinBox();
	densityScaleSpinner_->setMinimum(0.01);
	densityScaleSpinner_->setMaximum(100);
	densityScaleSpinner_->setDecimals(2);
	densityScaleSpinner_->setSingleStep(0.1);
	densityScaleSpinner_->setValue(renderer_->getSettings().densityScale_);
	connect(densityScaleSpinner_, SIGNAL(valueChanged(double)), this, SLOT(setDensityScale(double)));
	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->addWidget(densityScaleLabel);
	layout2->addWidget(densityScaleSpinner_, 1);
	layoutMain->addLayout(layout2);

	QLabel* blendModeLabel = new QLabel("Blend mode:  ");
	blendModeComboBox_ = new QComboBox();
	blendModeComboBox_->addItem("Alpha");
	blendModeComboBox_->addItem("Additive");
	blendModeComboBox_->addItem("Add Density");
	blendModeComboBox_->addItem("Max Density");
	blendModeComboBox_->setCurrentIndex((int) renderer_->getSettings().blendMode_);
	connect(blendModeComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendMode(int)));
	QHBoxLayout* layout3 = new QHBoxLayout();
	layout3->addWidget(blendModeLabel);
	layout3->addWidget(blendModeComboBox_, 1);
	layoutMain->addLayout(layout3);
	
	showSliceCheckBox_ = new QCheckBox("Show slice");
	showSliceCheckBox_->setChecked(renderer_->getSettings().showSlice_);
	connect(showSliceCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(setShowSlice(int)));
	fixSliceOrientationCheckBox_ = new QCheckBox("Fix orientation");
	fixSliceOrientationCheckBox_->setChecked(fixSliceOrientation_);
	connect(fixSliceOrientationCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(setFixSliceOrientation(int)));
	QHBoxLayout* layout4 = new QHBoxLayout();
	layout4->addWidget(showSliceCheckBox_);
	layout4->addWidget(fixSliceOrientationCheckBox_);
	layoutMain->addLayout(layout4);

	sliceSlider_ = new QSlider(Qt::Orientation::Horizontal);
	sliceSlider_->setMaximum((int)(-0.5 * SLICE_SLIDER_SCALE));
	sliceSlider_->setMaximum((int)(0.5 * SLICE_SLIDER_SCALE));
	sliceSlider_->setValue(sliceDistance_ * SLICE_SLIDER_SCALE);
	sliceSlider_->setTickInterval(1);
	sliceSlider_->setToolTip("The slice position");
	connect(sliceSlider_, SIGNAL(valueChanged(int)), this, SLOT(setSlice(int)));
	layoutMain->addWidget(sliceSlider_);

	//not needed anylonger
	//cameraTestButton_ = new QPushButton("Test Camera");
	//connect(cameraTestButton_, &QPushButton::clicked, this, &VolumeRendererWidget::testCamera);
	//layoutMain->addWidget(cameraTestButton_);

	//newTransferFunctionButton_ = new QPushButton("New Transfer Function");
	//connect(newTransferFunctionButton_, &QPushButton::clicked, this, &VolumeRendererWidget::changeTransferFunction);
	//layoutMain->addWidget(newTransferFunctionButton_);

	box->setLayout(layoutMain);
}

void VolumeRendererWidget::setVolume(Volume_ptr vol)
{
	//clone it and normalize it
	volume_ = std::make_shared<Volume>(vol->getBoundingBox().min(), vol->getBoundingBox().max(), vol->getSpacing(), vol->getNumVoxels());
	float min = vol->getContent().minCoeff();
	float max = vol->getContent().maxCoeff();
	float scale = 1 / (max - min);
	volume_->setContent((vol->getContent().array() - min) * scale);

	renderer_->getSettings().volume_ = volume_;
	renderer_->getSettings().sliceHelper_ = SliceHelper(Eigen::AlignedBox3f(
		volume_->getBoundingBox().min(),
		volume_->getBoundingBox().max() - volume_->getSpacing()));
	renderer_->notifyChange(VolumeRenderer::FLAG_VOLUME);
}

void VolumeRendererWidget::setCamera(const Camera& camera)
{
	camera_ = camera;
	renderer_->getSettings().camera_ = camera_;
	if (renderer_->getSettings().showSlice_) {
		computeSliceNormal();
		renderer_->getSettings().sliceHelper_.setSlice(sliceOrientation_, sliceDistance_);
		renderer_->notifyChange(VolumeRenderer::FLAG_CAMERA | VolumeRenderer::FLAG_SLICE);
	}
	else {
		renderer_->notifyChange(VolumeRenderer::FLAG_CAMERA);
	}
}

void VolumeRendererWidget::setTransferFunction(const ITransferFunction* const func)
{
	transferFunc_ = func;
	if (testTransferFunction_ != func) testTransferFunction_ = nullptr;
	renderer_->getSettings().transferFunction_ = func;
	renderer_->notifyChange(VolumeRenderer::FLAG_TRANSFER_FUNCTION);
}

void VolumeRendererWidget::onImageChanged()
{
	//redraw
	update();
}

Eigen::Vector2i VolumeRendererWidget::getScreenSize()
{
	return Eigen::Vector2i(width(), height());
}

void VolumeRendererWidget::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	renderer_->paintEvent(p);
}

void VolumeRendererWidget::resizeEvent(QResizeEvent* event)
{
	Eigen::Vector2i size(event->size().width(), event->size().height());

	//set camera size
	//TODO: remove this when the camera is fully implemented
	//these two lines
	//camera_.setScreenSize(size);
	//renderer_->getSettings().camera_ = camera_;

	renderer_->getSettings().screenSize_ = size;
	renderer_->notifyChange(VolumeRenderer::FLAG_SCREEN_SIZE);

	//fire event
	emit screenSizeChanged(size); //TODO: when the camera is fully implemented, this will update the camera
}

void VolumeRendererWidget::setShowOutline(int showOutline)
{
	renderer_->getSettings().showBoundingBox_ = showOutline == Qt::CheckState::Checked;
	renderer_->notifyChange(VolumeRenderer::FLAG_BOUNDING_BOX);
}

void VolumeRendererWidget::setStepSize(double stepSize)
{
	renderer_->getSettings().stepSize_ = stepSize;
	renderer_->notifyChange(VolumeRenderer::FLAG_STEP_SIZE);
}

void VolumeRendererWidget::setDensityScale(double scale)
{
	renderer_->getSettings().densityScale_ = scale;
	renderer_->notifyChange(VolumeRenderer::FLAG_DENSITY_SCALE);
}

void VolumeRendererWidget::setBlendMode(int blendMode)
{
	renderer_->getSettings().blendMode_ = (VolumeRenderer_BlendMode) blendMode;
	renderer_->notifyChange(VolumeRenderer::FLAG_BLEND_MODE);
}

void VolumeRendererWidget::setShowSlice(int showSlice)
{
	renderer_->getSettings().showSlice_ = showSlice == Qt::Checked;
	if (renderer_->getSettings().showSlice_) {
		computeSliceNormal();
		renderer_->getSettings().sliceHelper_.setSlice(sliceOrientation_, sliceDistance_);
	}
	renderer_->notifyChange(VolumeRenderer::FLAG_SLICE);
}

Eigen::Vector3f VolumeRendererWidget::computeSliceNormal()
{
	if (!fixSliceOrientation_)
	{
		sliceOrientation_ = (renderer_->getSettings().camera_.getWorldCoordinates(width() / 2, height() / 2, 1)
			- renderer_->getSettings().camera_.getWorldCoordinates(width() / 2, height() / 2, 0))
		.normalized();
	}
	return sliceOrientation_;
}

void VolumeRendererWidget::setFixSliceOrientation(int fixOrientation)
{
	fixSliceOrientation_ = fixOrientation == Qt::Checked;
	if (!fixSliceOrientation_)
	{
		computeSliceNormal();
		renderer_->getSettings().sliceHelper_.setSlice(sliceOrientation_, sliceDistance_);
		renderer_->notifyChange(VolumeRenderer::FLAG_SLICE);
	}
}

void VolumeRendererWidget::setSlice(int slice)
{
	sliceDistance_ = slice * volume_->getBoundingBox().diagonal().norm() / (float)SLICE_SLIDER_SCALE;
	renderer_->getSettings().sliceHelper_.setSlice(sliceOrientation_, sliceDistance_);
	renderer_->notifyChange(VolumeRenderer::FLAG_SLICE);
}

void VolumeRendererWidget::testCamera()
{
	setCamera(Camera::createTestCamera(Eigen::Vector2i(width(), height())));
}

void VolumeRendererWidget::changeTransferFunction()
{
	delete testTransferFunction_;
	testTransferFunction_ = TestTransferFunction::createRandom();
	setTransferFunction(testTransferFunction_);
}
