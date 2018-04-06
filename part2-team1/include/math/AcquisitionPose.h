#ifndef ACQUISITION_POSE_H
#define ACQUISITION_POSE_H

#include <Eigen/Dense>
#include <math/Volume.h>
#include <math/Detector.h>
#include <qmetatype.h>

/**
 * \brief Stores a single acquisition pose.
 * The class is designed in such a way that the same pose (i.e. the same detector settings)
 * can be used for multiple volumes. Only a different volume has to be passed to compute(Volume_ptr).
 */
class AcquisitionPose
{
private:
	/**
	 * \brief The detector stores all the settings needed to evalute and simulate the image
	 */
	Detector detector_;

	/**
	 * \brief The output of the raytracing.
	 * It will have the size of detector_.getResolution().x() * detector_.getResolution().y().
	 * The output image is linearized as a vector in the following way:
	 *   index = x + y*width
	 */
	Eigen::VectorXf output_;

public:

	AcquisitionPose() {} //default constructor, needed for Qt

	explicit AcquisitionPose(const Detector& detector)
		: detector_(detector)
	{
	}

	Detector& getDetector() { return detector_; }
	const Detector& getDetector() const { return detector_; }
	void setDetector(const Detector& detector) { detector_ = detector; }

	const Eigen::VectorXf& getOutput() const { return output_; }

	/**
	 * \brief Computes the simulated X-Ray image.
	 * It calls Raytracer::raytrace for each ray obtained by Detector::computeRays()
	 * \param volume the input volume
	 */
	void compute(Volume_ptr volume);
};

//Needed to send it by signals and slots
Q_DECLARE_METATYPE(AcquisitionPose)

#endif
