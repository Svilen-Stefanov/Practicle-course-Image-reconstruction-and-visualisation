#ifndef VOLUME_RENDERER_WIDGET_H
#define VOLUME_RENDERER_WIDGET_H

#include <QtWidgets>
#include <Eigen/Dense>

#include <math/Volume.h>
#include <math/ITransferFunction.h>
#include <math/Camera.h>
#include <math/VolumeRenderer.h>

class VolumeRendererWidget : public QWidget
{
	Q_OBJECT;

private:
	Camera camera_;
	const ITransferFunction* transferFunc_;
	Volume_ptr volume_;
	bool fixSliceOrientation_;
	Eigen::Vector3f sliceOrientation_;
	float sliceDistance_;
	static const int SLICE_SLIDER_SCALE = 100;

	VolumeRenderer* renderer_;
	QImage img_;

public:
	VolumeRendererWidget(QWidget* parent = nullptr);
	~VolumeRendererWidget();

	void setupGui(QGroupBox* box);

public slots:
	/**
	 * \brief Sets the volume to be rendererd
	 * \param vol the volume to render
	 */
	void setVolume(Volume_ptr vol);

	/**
	 * \brief Sets the camera
	 * \param camera the camera
	 */
	void setCamera(const Camera& camera);

	/**
	 * \brief Sets the transfer function, created by the TransferFunctionEditor class.
	 * \param func the transfer function
	 */
	void setTransferFunction(const ITransferFunction* const func);

	/**
	 * \brief This slot receives the signal 'sendImageChanged' from VolumeRenderer.
	 */
	void onImageChanged();

signals:

	/**
	 * \brief Called when the screen size is changed because the window is resized.
	 * Should be connected to Camera::setScreenSize(screenSize)
	 * \param screenSize the new screen size
	 */
	void screenSizeChanged(const Eigen::Vector2i& screenSize);

public:
	/**
	 * \brief Retrieves the current screen size.
	 * Needed maybe to initialize the Camera
	 * \return the screen size
	 */
	Eigen::Vector2i getScreenSize();

protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

	// ----------------------
	// GUI
	// ----------------------
private:
	QCheckBox* showOutlineCheckBox_;
	QDoubleSpinBox* stepSizeSpinner_;
	QDoubleSpinBox* densityScaleSpinner_;
	QComboBox* blendModeComboBox_;
	QCheckBox* showSliceCheckBox_;
	QCheckBox* fixSliceOrientationCheckBox_;
	QSlider* sliceSlider_;

	//Test
	QPushButton* cameraTestButton_;
	QPushButton* newTransferFunctionButton_;
	ITransferFunction* testTransferFunction_;

	Eigen::Vector3f computeSliceNormal();
private slots:
	void setShowOutline(int showOutline);
	void setStepSize(double stepSize);
	void setDensityScale(double scale);
	void setBlendMode(int blendMode);
	void setShowSlice(int showSlice);
	void setFixSliceOrientation(int fixOrientation);
	void setSlice(int slice);

	//Test
	void testCamera();
	void changeTransferFunction();
};

#endif
