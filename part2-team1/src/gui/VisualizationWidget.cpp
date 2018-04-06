#include <QtWidgets>
#include <iostream>
#include <sstream>

#include <gui/VisualizationWidget.h>
#include <math/Camera.h>

VisualizationWidget::VisualizationWidget(QWidget *parent) :
      rotationDegreeX_(175),
      rotationDegreeY_(165), 
      mousePressed_(false)
{
	rendererWidget_ = new VolumeRendererWidget(this);
  	transferFuncWidget_ = new TransferFunctionWidget();

	QHBoxLayout* layoutMain = new QHBoxLayout;
	QVBoxLayout* groupLayout = new QVBoxLayout;

	groupLayout->addWidget(cameraGroup());
	groupLayout->addWidget(rendererGroup());
	groupLayout->addWidget(TransferFunctionGroup(), 1);

	layoutMain->addLayout(groupLayout,0);
	layoutMain->addWidget(rendererWidget_, 1);

	setLayout(layoutMain);
	setMouseTracking(true);
	connectSlots();
	setDefaultCameraSettings();
}

void VisualizationWidget::connectSlots() {
	//rotation
	connect(rotationXDial_, SIGNAL(valueChanged(int)), SLOT(setXRotation(int)));
	connect(rotationYDial_, SIGNAL(valueChanged(int)), SLOT(setYRotation(int)));

	//default camera settings
	connect(resetCameraSettings_,SIGNAL(clicked()),SLOT(setDefaultCameraSettings()));

	//snap camera
	connect(snapCameraButton_, SIGNAL(clicked()), SLOT(snapCamera()));

  	//transfer function
	connect(transferFuncWidget_, SIGNAL(sendTransferFunction(const ITransferFunction* const)), 
		rendererWidget_, SLOT(setTransferFunction(const ITransferFunction* const)));

	//perspective and orthogonal projection
	connect(perspectivProjRadioBut_,SIGNAL(clicked()),this, SLOT(setPersProjValue()));
	connect(orthogonalProjRadioBut_,SIGNAL(clicked()),this,SLOT(setOrthProjValue()));

	//connect camera matrix with the volume renderer
	connect(this,SIGNAL(cameraEmitSignal(Camera)),rendererWidget_,SLOT(setCamera(Camera)));
	connect(rendererWidget_,SIGNAL(screenSizeChanged(const Eigen::Vector2i&)),this,SLOT(screenSizeUpdated(const Eigen::Vector2i&)));
}

void VisualizationWidget::screenSizeUpdated(const Eigen::Vector2i& screenSize){
	cam_.setScreenSize(screenSize);
	emit(cameraEmitSignal(cam_));
}

void VisualizationWidget::setPersProjValue() {
	cam_.setPerspectiveProject(true);
	emit(cameraEmitSignal(cam_));
}

void VisualizationWidget::setYRotation(int rot)
{
	if (rot != rotationYDial_->value())
	{
		rotationYDial_->setValue(rot);
	}
	rotationDegreeY_ = rot;
	cam_.setRotationY(rotationDegreeY_ * M_PI / 180);
	emit cameraEmitSignal(cam_);
}

void VisualizationWidget::setXRotation(int rot)
{
	if (rot != rotationXDial_->value()) {
		rotationXDial_->setValue(rot);
	}
	rotationDegreeX_ = rot;
	cam_.setRotationX(rotationDegreeX_ * M_PI / 180);
	emit cameraEmitSignal(cam_);
}

void VisualizationWidget::snapCamera()
{
	setXRotation(45 * (int)std::round(rotationDegreeX_ / 45.0));
	setYRotation(45 * (int)std::round(rotationDegreeY_ / 45.0));
}

void VisualizationWidget::setOrthProjValue() {
	cam_.setPerspectiveProject(false);
	emit(cameraEmitSignal(cam_));
}
void VisualizationWidget::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_W) {			// rotate positive around x
		setYRotation(rotationDegreeY_ + 2);
    } else if (event->key() == Qt::Key_S) { // rotate in negative direction around x
		setYRotation(rotationDegreeY_ - 2);
    } else if (event->key() == Qt::Key_A) {//rotate left around y
		setXRotation(rotationDegreeX_ + 2);
    } else if (event->key() == Qt::Key_D) { //roate right around y
		setXRotation(rotationDegreeX_ - 2);
    } else if (event->key() == Qt::Key_Q) { //translate z out
    	cam_.setTranslation(-1);
    	emit(cameraEmitSignal(cam_));
    } else if (event->key() == Qt::Key_E) { //translate z in
    	cam_.setTranslation(1);
    	emit(cameraEmitSignal(cam_));
    }
    
}

void VisualizationWidget::wheelEvent(QWheelEvent *event){ // TO DO!!!!!
    if (event->orientation() == Qt::Vertical) {
        int scrollFactor =  event->delta()/120;
        if (scrollFactor != 0) {
        	cam_.setTranslation(scrollFactor);
        	emit(cameraEmitSignal(cam_));
        } 
    }
}

VisualizationWidget::~VisualizationWidget() {
    delete transferFuncWidget_;
   }

void VisualizationWidget::setDefaultCameraSettings() {
    rotationDegreeX_ = 175;
    rotationDegreeY_ = 165;
    perspectivProjRadioBut_->setChecked(true);
    setPersProjValue();
	setXRotation(175);
	setYRotation(165);
	cam_.setDefaultCameraValues();
	emit(cameraEmitSignal(cam_));
}

void VisualizationWidget::mousePressEvent(QMouseEvent *event) {
	if(Qt::LeftButton == event->button()) {
		mousePressed_ = true;
		lastMousePos_ = event->pos();
	}
}

void VisualizationWidget::mouseReleaseEvent(QMouseEvent *event) {
	if(Qt::LeftButton == event->button()) {
		mousePressed_ = false;
	}
}

void VisualizationWidget::mouseMoveEvent(QMouseEvent *event){
    if (mousePressed_) {
        QPoint diff = lastMousePos_ - event->pos();
        float x = rotationDegreeX_ + diff.x();
        float y = rotationDegreeY_ + diff.y();
		setXRotation(x);
		setYRotation(y);
        lastMousePos_ = event->pos();
    }
}

void VisualizationWidget::setVolume(Volume_ptr vol)
{
	vol_ = vol;
	rendererWidget_->setVolume(vol);
	cam_.setBoundingBox(vol->getBoundingBox());
}

QGroupBox* VisualizationWidget::cameraGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Camera Settings"));
	QVBoxLayout* layoutMain = new QVBoxLayout();
	layoutMain->setContentsMargins(3, 3, 3, 3);
	layoutMain->setSpacing(3);

	orthogonalProjRadioBut_ = new QRadioButton("Orthogonal Projection");
	perspectivProjRadioBut_ = new QRadioButton("Perspective Projection");
	perspectivProjRadioBut_->setChecked(true);

	QLabel* rotationXLabel = new QLabel("X-Rot");
	rotationXLabel->setAlignment(Qt::AlignHCenter);
	QLabel* rotationYLabel = new QLabel("Y-Rot");
	rotationYLabel->setAlignment(Qt::AlignHCenter);
	rotationXDial_ = new QDial(this);
	rotationXDial_->setMinimum(0);
	rotationXDial_->setMaximum(359);
	rotationXDial_->setWrapping(true);
	rotationXDial_->setPageStep(45);
	rotationXDial_->setValue(rotationDegreeX_);
	rotationXDial_->setFixedHeight(50);
	rotationYDial_ = new QDial(this);
	rotationYDial_->setMinimum(0);
	rotationYDial_->setMaximum(359);
	rotationYDial_->setWrapping(true);
	rotationYDial_->setPageStep(45);
	rotationYDial_->setValue(rotationDegreeY_);
	rotationYDial_->setFixedHeight(50);
	QGridLayout* rotLayout = new QGridLayout();
	rotLayout->addWidget(rotationXLabel, 0, 0);
	rotLayout->addWidget(rotationYLabel, 0, 1);
	rotLayout->addWidget(rotationXDial_, 1, 0);
	rotLayout->addWidget(rotationYDial_, 1, 1);

	layoutMain->addWidget(perspectivProjRadioBut_);
	layoutMain->addWidget(orthogonalProjRadioBut_);

	layoutMain->addLayout(rotLayout);

	QHBoxLayout* l = new QHBoxLayout();
	resetCameraSettings_ = new QPushButton("Reset");
	l->addWidget(resetCameraSettings_);

	snapCameraButton_ = new QPushButton("Snap camera");
	snapCameraButton_->setToolTip("Snaps the camera rotation to a multiple of 45°");
	l->addWidget(snapCameraButton_);

	layoutMain->addLayout(l);

	groupBox->setLayout(layoutMain);
	return groupBox;
}

QGroupBox* VisualizationWidget::rendererGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Renderer Settings"));
	rendererWidget_->setupGui(groupBox);
	return groupBox;
}

QGroupBox* VisualizationWidget::TransferFunctionGroup() {
	QGroupBox* groupBox = new QGroupBox(tr("Transfer Functions"));
	transferFuncWidget_->setupGui(groupBox);
	return groupBox;
}
