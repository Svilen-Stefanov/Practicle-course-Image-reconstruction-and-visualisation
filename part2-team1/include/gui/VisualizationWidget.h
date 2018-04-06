#ifndef VIS_WIDGET_H
#define VIS_WIDGET_H

#include <QLabel>
#include <QtWidgets>
#include <QImage>

#include <gui/VolumeRendererWidget.h>
#include <gui/TransferFunctionWidget.h>
#include <math/Camera.h>

class VisualizationWidget : public QWidget {

	Q_OBJECT;

public:
	VisualizationWidget(QWidget* parent = nullptr);
	~VisualizationWidget();

private:
	Volume_ptr vol_;
	VolumeRendererWidget* rendererWidget_;
	TransferFunctionWidget* transferFuncWidget_;
	Camera cam_;
	Eigen::Vector2i aspectRatio_;

	//ui for camera
	QRadioButton* orthogonalProjRadioBut_;
	QRadioButton* perspectivProjRadioBut_;
	QDial* rotationXDial_;
	QDial* rotationYDial_;
	QPushButton* resetCameraSettings_;
	QPushButton* snapCameraButton_;

	//camera values
	QPoint m_lastPos_; //pos for angles
    //rotation values in degrees
    int rotationDegreeX_;
    int rotationDegreeY_;

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyReleaseEvent(QKeyEvent* event);
	void wheelEvent(QWheelEvent *event);

	//for mouse events
	bool mousePressed_;
	QPoint lastMousePos_;

	void connectSlots();
	//gui stuff
	QGroupBox* cameraGroup();
	QGroupBox* rendererGroup();
	QGroupBox* TransferFunctionGroup();

public slots:
	void setVolume(Volume_ptr vol);
	void setDefaultCameraSettings();
	void setOrthProjValue();
	void setPersProjValue();
	void setYRotation(int);
	void setXRotation(int);
	void snapCamera();
	void screenSizeUpdated(const Eigen::Vector2i& screenSize);
	
signals:
	void signalAspectRatio(Eigen::Vector2i);
	void rotationXEvent(int); //mapped to x value of mouse
	void rotationYEvent(int); //mapped to y value of mouse
	void cameraEmitSignal(Camera cam);
};


#endif
