#include <math/AcquisitionPoses.h>

AcquisitionPoses::AcquisitionPoses(const Eigen::AlignedBox3f& bounding_box)
	: boundingBox_(bounding_box)
{
}
