#ifndef SLICE_HELPER_H
#define SLICE_HELPER_H

#include <Eigen/Dense>
#include <vector>
#include <math/Volume.h>
#include <math/Raytracer.h>

class SliceHelper
{
private:
	Eigen::AlignedBox3f box_;
	Eigen::Vector4f plane_;

public:
	/**
	 * \brief Creates a slice helper for the given volume bounding box
	 * \param box the bounding box
	 */
	SliceHelper(const Eigen::AlignedBox3f& box);

	/**
	 * \brief Sets the current slice
	 * \param normal the normal of the slice plane, normalized
	 * \param distanceFromCenter the distance of the slice from the center of the volume (in world coordinates)
	 */
	void setSlice(const Eigen::Vector3f normal, float distanceFromCenter);

	/**
	 * \brief Computes the intersection polygon of the slice with the bounding box
	 * \return the vertices of the box in world coordinates
	 */
	std::vector<Eigen::Vector3f> computeIntersectionPolygon() const;

	/**
	 * \brief Computes the intersection of a ray with the slice plane
	 * \param rayStart the ray start
	 * \param rayDirection the ray direction
	 * \param result the resulting collision position
	 * \return true if the ray collided inside the bounding box
	 */
	bool intersectRay(const Eigen::Vector3f& rayStart, const Eigen::Vector3f& rayDirection, Eigen::Vector3f& result) const;

private:
	bool ray_to_plane(const Eigen::Vector3f& rayOrig, const Eigen::Vector3f& rayDir, float* outT, float* outVD) const;
	std::vector<Eigen::Vector3f> calc_plane_aabb_intersection_points() const;
	void sort_points(std::vector<Eigen::Vector3f>& points) const;
};

#endif