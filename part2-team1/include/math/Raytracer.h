#ifndef AC_RAYTRACER_H
#define AC_RAYTRACER_H

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <vector>
#include <algorithm>
#include <cmath>

#include <math/Volume.h>
#include <math/Detector.h> //for the definition of Ray_t


/**
 * \brief The raytracer for the acquisition step.
 * Takes a volume and a ray and computes the forward projection.
 */
class Raytracer
{
private:
	Volume_ptr volume_;
	Detector::Ray_t ray_;
	Eigen::Vector2f collisionResult_;
	
	//voxel traversal variables
	int startX, startY, startZ;
	int endX, endY, endZ;
	int stepX, stepY, stepZ;
	int justOutX, justOutY, justOutZ;
	float tMaxX, tMaxY, tMaxZ;
	float tDeltaX, tDeltaY, tDeltaZ;

public:
	
	/**
	 * \brief Constructs a new raytracer tracing the specified volume
	 * \param volume the volume to trace
	 */
	explicit Raytracer(const Volume_ptr volume, const Detector::Ray_t& ray);

	/**
	 * \brief Performs a raytracing with the specified ray.
	 * Internially, it first calls rayBoxIntersection(const Detector::Ray_t&)
	 * and then voxelTraversal(const Detector::Ray_t&, float).
	 * \return the accumulated voxel values
	 */
	float forwardProjection() const;
	/**
	* \brief Performs a raytracing with the specified ray.
	* Internially, it first calls rayBoxIntersection(const Detector::Ray_t&)
	* and then voxelTraversal(const Detector::Ray_t&, float).
	* \param vol the content of the volume
	* \return the accumulated voxel values
	*/
	float forwardProjection(const Eigen::VectorXf& vol) const;

	/**
	 * \brief Performs the backward projection.
	 * It takes the value and adds it to all traversed cells in the specified volume.
	 * The resolution of the volume specified in the constructor and the volume passed as parameter must match.
	 * (In order that this makes sense, also the bounding box must match)
	 * \param value the value to distribute to the volume
	 */
	void backwardProjection(float value) const;
	/**
	* \brief Performs the backward projection.
	* It takes the value and adds it to all traversed cells in the specified volume.
	* The resolution of the volume specified in the constructor and the volume passed as parameter must match.
	* (In order that this makes sense, also the bounding box must match)
	* \param value the value to distribute to the volume
	* \param vol the content of the volume
	*/
	void backwardProjection(float value, Eigen::VectorXf& vol) const;

	/**
	 * \brief Tests if the specified ray hits the bounding box.
	 * \param ray the ray
	 * \param boundingBox the bounding box 
	 * \return -1 if the ray does not hit the box, otherwise it returns the distance to the first hit and to the last hit
	 */
	static Eigen::Vector2f rayBoxIntersection(const Detector::Ray_t& ray, const Eigen::AlignedBox3f& boundingBox);

public: //public for testing only

	/**
	 * \brief Tests if the specified ray hits the bounding box of the volume.
	 * \param ray the ray
	 * \return -1 if the ray does not hit the box, otherwise it returns the distance to the first hit
	 */
	Eigen::Vector2f rayBoxIntersection(const Detector::Ray_t& ray) const;

	/**
	 * \brief Performs a voxel traversal through the volume
	 * \return the accumulated voxel values
	 */
	float voxelTraversal(const Eigen::VectorXf& vol) const;

	void voxelTraversalBackwards(float val, Eigen::VectorXf& vol) const;

	void setupTraversal(const Detector::Ray_t& ray, float entry, float exit);

	template<typename T>
	static inline T clamp(const T& value, const T& min, const T& max)
	{
		return std::min(std::max(value, min), max);
	}
	static inline float frac(float value)
	{
		float dummy;
		float f = std::modf(value, &dummy);
		if (f < 0) f += 1;
		return f;
	}
	template <typename T> 
	static inline int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}
};


#endif