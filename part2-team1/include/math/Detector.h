#ifndef DETECTOR_H
#define DETECTOR_H

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <vector>

/**
 * \brief A detector stores all properties to uniquely represent a detector and source in the 3D space.
 * In the acquisition step, an image of the volume is taken from the perspective of the detector.
 * The reconstruction then takes these images and the inverse projection of the detector to reconstruct the volume.
 * 
 * This is how the rays are computed:
 * Assume an identity rotation and the center is located at (0,0,0)
 * The x-ray source is then located at (-distance, 0, 0)
 * The detector center is located at (+distance, 0, 0) and it spans a quad in the YZ plane (axis aligned)
 * with a side length of size.
 * Then, the rotation and after that the translation to center.
 */
class Detector
{
private:
	// core properties, accessed by setters

	/**
	 * \brief The size of the 2D projector plane.
	 * The normal of that plane always points towards the source.
	 */
	Eigen::Vector2f size_;

	/**
	 * \brief The resolution of the detector, i.e. the number of texels.
	 */
	Eigen::Vector2i resolution_;

	/**
	 * \brief The rotation center
	 */
	Eigen::Vector3f center_;

	/**
	 * \brief The distance from the center to the detector.
	 * The source lies exactly at the oposite site.
	 */
	float distance_;

	/**
	 * \brief The rotation of the detector around the center.
	 */
	Eigen::Quaternionf rotation_;

	// derieved properties, for caching
	//   (not needed yet)
	//Eigen::Vector3f sourcePosition_;
	//Eigen::Vector3f detectorPosition_;


public:
	Detector(const Eigen::Vector2f& size = Eigen::Vector2f(0.2, 0.2),
	         const Eigen::Vector2i& resolution = Eigen::Vector2i(5, 5),
	         const Eigen::Vector3f& center = Eigen::Vector3f(0, 0, 0),
	         float distance = 0.4,
	         const Eigen::Quaternionf& rotation = Eigen::Quaternionf::Identity());

	const Eigen::Vector2f& getSize() const { return size_; }

	void setSize(const Eigen::Vector2f& size);
	
	const Eigen::Vector2i& getResolution() const { return resolution_; }

	void setResolution(const Eigen::Vector2i& resolution);

	const Eigen::Vector3f& getCenter() const { return center_; }

	void setCenter(const Eigen::Vector3f& center);

	float getDistance() const { return distance_; }

	void setDistance(float distance);

	const Eigen::Quaternionf& getRotation() const { return rotation_; }

	void setRotation(const Eigen::Quaternionf& rotation);

	/**
	 * \To prevent the special cases where we might have a coordinate right at 0.
	 * \This checks if any coordinates have a zero, and if so replaces with very small value
	 */
    static Eigen::Vector3f checkForZero(Eigen::Vector3f vec);

	/**
	 * \brief Defines a single ray.
	 */
	typedef Eigen::ParametrizedLine<float, 3> Ray_t;
	/**
	 * \brief Computes the rays that point from each detector cell to the source.
	 *        The rays are indexed so that the following expression is valid: rays[x + resolution.x()*y]
	 * \return a vector of all the rays
	 */
	std::vector<Ray_t> computeRays() const;
};

#endif