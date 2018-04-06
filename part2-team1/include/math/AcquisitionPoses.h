#ifndef ACQUISITION_POSES_H
#define ACQUISITION_POSES_H

#include <vector>
#include <Eigen/Geometry>
#include <math/AcquisitionPose.h>

/**
 * \brief The acquisition poses contains the output of the image acquisition step. It is passed on to the reconstruction.
 * It stores a vector of all acquisition poses with their outputs generated, and the bounding box of the observed object.
 */
class AcquisitionPoses
{
private:
	std::vector<AcquisitionPose> poses_;
	Eigen::AlignedBox3f boundingBox_;


public:
	AcquisitionPoses() {}
	/**
	 * \brief Constructs a new instance with the specified bounding box
	 * \param bounding_box the bounding box of the volume
	 */
	explicit AcquisitionPoses(const Eigen::AlignedBox3f& bounding_box);

	const std::vector<AcquisitionPose>& getPoses() const { return poses_; }
	std::vector<AcquisitionPose>& getPoses() { return poses_; }

	const Eigen::AlignedBox3f& getBoundingBox() const { return boundingBox_; }
	void setBoundingBox(Eigen::AlignedBox3f& box) { boundingBox_ = box; }
};

//Needed to send it by signals and slots
Q_DECLARE_METATYPE(AcquisitionPoses)

#endif
