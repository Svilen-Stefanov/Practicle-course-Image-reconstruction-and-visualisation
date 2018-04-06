#ifndef VOLUME_RENDERER_H
#define VOLUME_RENDERER_H

#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <QtWidgets>

#include <math/Volume.h>
#include <math/ITransferFunction.h>
#include <math/Camera.h>
#include <math/SliceHelper.h>

/*
 * I used the concepts from Qt's Mandelbrot example to implement the parallelism.
 * But I extended it with a finer grained approach using worker threads.
 */

//--------------------------------------------------------------
//  HELPER CLASSES
//--------------------------------------------------------------
/**
* \brief The blend mode.
* This blend mode determine how the voxels along are ray are blended together
*/
enum VolumeRenderer_BlendMode
{
	/**
	* \brief Regular front-to-back alpha blending
	*/
	ALPHA,
	/**
	* \brief Additive blending, the colors are added up
	*/
	ADDITIVE,
	/**
	 * \brief The densitites are added up and then converted to a color 
	 */
	ADDITIVE_DENSITY,
	/**
	 * \brief The maximal density value is used and then converted to a color
	 */
	MAX_DENSITY
};

/**
* \brief The render settings
*/
struct VolumeRenderer_RenderSettings
{
public:
	/**
	* \brief The volume
	*/
	Volume_ptr volume_;
	/**
	* \brief The transfer funcion
	*/
	const ITransferFunction* transferFunction_;
	/**
	* \brief The camera
	*/
	Camera camera_;
	/**
	* \brief the screen size
	*/
	Eigen::Vector2i screenSize_;
	/**
	* \brief True iff a bounding box around the volume should be shown
	*/
	bool showBoundingBox_;
	/**
	* \brief The step size of the raytracer.
	* A higher step size leads to a more precise output, but also to a higher computational time
	*/
	float stepSize_;
	/**
	* \brief A scaling factor to the alpha values.
	* Lower values allow the ray to pass through the volume for a longer time
	*/
	float densityScale_;
	/**
	* \brief The blend mode
	*/
	VolumeRenderer_BlendMode blendMode_;
	/**
	 * \brief True if a slice should be rendered instead of the whole volume
	 */
	bool showSlice_;
	/**
	 * \brief The slice helper, stores the information about the selected slice
	 */
	SliceHelper sliceHelper_;

	VolumeRenderer_RenderSettings();
};

typedef size_t VolumeRenderer_flag_t;

//--------------------------------------------------------------
//  PRIVATE CLASSES
//--------------------------------------------------------------
/**
* \brief Information about a single ray
*/
struct _VolumeRenderer_RayInfo
{
	bool rayComputed_;
	Eigen::Vector3f originalStart_;
	Eigen::Vector3f originalDirection_;
	Eigen::Vector3f start_; //start position (in the volume)
	Eigen::Vector3f direction_; //normalized direction
	float maxStep_; //maximal step distance

	_VolumeRenderer_RayInfo() : rayComputed_(false) {}

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

/**
* \brief Describes one single piece of work
*/
struct _VolumeRenderer_WorkItem
{
	Eigen::Vector2i start_, end_; //the covered area
	std::vector<_VolumeRenderer_RayInfo, Eigen::aligned_allocator<_VolumeRenderer_RayInfo>> rays_;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class _VolumeRenderer_WorkerThread : public QThread
{
	Q_OBJECT;
};

/**
* \brief This thread manages the multiple worker threads, it distributes the work to the workers
* and merges the result
*/
class _VolumeRenderer_ManagerThread : public QThread
{
	Q_OBJECT;

public:
	_VolumeRenderer_ManagerThread();
	~_VolumeRenderer_ManagerThread();

	void notifyChange(const VolumeRenderer_RenderSettings& settings, VolumeRenderer_flag_t flags);

protected:
	void run() override;

signals:
	void sendImageChanged(const QImage& img);

private:
	static const int WORK_ITEM_SIZE = 16;
	void setupWorkItems(const VolumeRenderer_RenderSettings& settings);
	void changeWorkItems(const VolumeRenderer_RenderSettings& settings, VolumeRenderer_flag_t flags);
	void refineWorkItem(int index, const VolumeRenderer_RenderSettings& settings, QRgb* colors, size_t scanline);
	bool advancePhase();
	void projectRay(_VolumeRenderer_RayInfo& ray, Volume_ptr vol) const; //projects the ray in voxel space
	Eigen::Vector4f computeRay(const _VolumeRenderer_RayInfo& ray, const VolumeRenderer_RenderSettings& settings);

private:
	QMutex mutex_;
	QWaitCondition condition_;
	bool abort_;
	bool restart_;
	VolumeRenderer_RenderSettings settings_;
	VolumeRenderer_flag_t flags_;

	std::vector<_VolumeRenderer_WorkItem, Eigen::aligned_allocator<_VolumeRenderer_WorkItem>> workItems_;
	int workPhase_;
	int workStep_;
};

//--------------------------------------------------------------
//  MAIN CLASSES
//--------------------------------------------------------------

class VolumeRenderer : public QObject
{
	Q_OBJECT;

private:
	//private flags, decode what should happen internially
	/**
	 * \brief Update only the bounding box, the raytracer results are the same
	 */
	static const VolumeRenderer_flag_t PFLAG_BOUNDING_BOX = 1;
	/**
	 * \brief Update slice
	 */
	static const VolumeRenderer_flag_t PFLAG_SLICE = 2;
	/**
	 * \brief The way how the voxels are traced have changed.
	 * (step size, density scale, blend mode, transfer function)
	 * The rays are the same
	 */
	static const VolumeRenderer_flag_t PFLAG_TRACING = 4;
	/**
	 * \brief Camera/Volume has changed. Recompute the rays
	 */
	static const VolumeRenderer_flag_t PFLAG_RAYS = 8;
	/**
	 * \brief The size of the preview window has changed. Recompute the parallel tasks
	 */
	static const VolumeRenderer_flag_t PFLAG_SIZE = 16;

public:
	//public flags
	static const VolumeRenderer_flag_t FLAG_BLEND_MODE = PFLAG_TRACING;
	static const VolumeRenderer_flag_t FLAG_DENSITY_SCALE = PFLAG_TRACING;
	static const VolumeRenderer_flag_t FLAG_STEP_SIZE = PFLAG_TRACING;
	static const VolumeRenderer_flag_t FLAG_TRANSFER_FUNCTION = PFLAG_TRACING;
	static const VolumeRenderer_flag_t FLAG_BOUNDING_BOX = PFLAG_BOUNDING_BOX;
	static const VolumeRenderer_flag_t FLAG_VOLUME = PFLAG_RAYS | PFLAG_TRACING;
	static const VolumeRenderer_flag_t FLAG_CAMERA = PFLAG_RAYS | PFLAG_TRACING;
	static const VolumeRenderer_flag_t FLAG_SCREEN_SIZE = PFLAG_SIZE | PFLAG_TRACING | PFLAG_RAYS;
	static const VolumeRenderer_flag_t FLAG_SLICE = PFLAG_SLICE;

private:
	VolumeRenderer_RenderSettings settings_;

public:
	VolumeRenderer();
	~VolumeRenderer();

	VolumeRenderer_RenderSettings& getSettings() { return settings_; }
	const VolumeRenderer_RenderSettings& getSettings() const { return settings_; }

	/**
	 * \brief Paints the volume.
	 * Should be called by the parent widget whenever the signal 'sendImageChanged()' is triggered
	 * \param event 
	 */
	void paintEvent(QPainter& painter);

public slots:
	/**
	 * \brief Notifies that some render settings have changed
	 * \param change a combination of the flags indicating what has changed
	 */
	void notifyChange(VolumeRenderer_flag_t change);

signals:
	/**
	 * \brief Notifies the parent widget that the output has changed.
	 * This should trigger a call to paintEvent(QPaintEvent*)
	 */
	void sendImageChanged();

private:

	_VolumeRenderer_ManagerThread* managerThread_;
	QImage img_;

	void paintSliceOutline(QPainter& painter);

private slots:
	void receiveImage(const QImage& img);

	friend _VolumeRenderer_ManagerThread;
};

#endif
