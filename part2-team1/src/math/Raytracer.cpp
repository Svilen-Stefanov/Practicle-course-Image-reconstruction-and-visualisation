#include <math/Raytracer.h>

#include <QDebug>

using namespace Eigen;

Raytracer::Raytracer(const Volume_ptr volume, const Detector::Ray_t& ray)
	: volume_(volume), ray_(ray)
{
	collisionResult_ = rayBoxIntersection(ray);
	if (collisionResult_[0] >= 0)
	{
		setupTraversal(ray, collisionResult_[0], collisionResult_[1]);
	}
}

float Raytracer::forwardProjection() const
{
	return forwardProjection(volume_->getContent());
}
float Raytracer::forwardProjection(const Eigen::VectorXf& vol) const
{
	if (collisionResult_[0] >= 0)
	{
		//hits the box
		return voxelTraversal(vol);
	} else
	{
		//completely outside
		return 0;
	}
}

void Raytracer::backwardProjection(float value) const
{
	backwardProjection(value, volume_->getContent());
}
void Raytracer::backwardProjection(float value, Eigen::VectorXf& vol) const
{
	if (collisionResult_[0] >= 0) {
		//hits the box
		voxelTraversalBackwards(value, vol);
	}
}

Eigen::Vector2f Raytracer::rayBoxIntersection(const Detector::Ray_t& ray, const Eigen::AlignedBox3f& boundingBox)
{
	//define a region for the intersection interval
	float t0 = 0;
	//Implement "An efficient and robust ray-box intersection algorithm"
	Eigen::Vector3f bounds[2];
	bounds[0] = boundingBox.min();
	bounds[1] = boundingBox.max();

	//Smits' improved method
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	float divx, divy, divz;

	divx = 1 / ray.direction().x();
	if (divx >= 0) {
		tmin = (bounds[0].x() - ray.origin().x()) * divx;
		tmax = (bounds[1].x() - ray.origin().x()) * divx;
	}
	else {
		tmin = (bounds[1].x() - ray.origin().x()) * divx;
		tmax = (bounds[0].x() - ray.origin().x()) * divx;
	}
	divy = 1 / ray.direction().y();
	if (divy >= 0) {
		tymin = (bounds[0].y() - ray.origin().y()) * divy;
		tymax = (bounds[1].y() - ray.origin().y()) * divy;
	}
	else {
		tymin = (bounds[1].y() - ray.origin().y()) * divy;
		tymax = (bounds[0].y() - ray.origin().y()) * divy;
	}

	//if there is no intersection
	if ((tmin > tymax) || (tymin > tmax)) {
		return Eigen::Vector2f(-1, -1);
	}

	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}

	//calculation in z direction
	divz = 1 / ray.direction().z();
	if (divz >= 0) {

		tzmin = (bounds[0].z() - ray.origin().z()) * divz;
		tzmax = (bounds[1].z() - ray.origin().z()) * divz;
	}
	else {
		tzmin = (bounds[1].z() - ray.origin().z()) * divz;
		tzmax = (bounds[0].z() - ray.origin().z()) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax)) {
		return Eigen::Vector2f(-1, -1);
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}

	if (tzmax < tmax) {
		tmax = tzmax;
	}

	if (tmax > t0) {
		float epsilon = std::numeric_limits<float>::epsilon();
		if (fabs(tmax - tmin) < epsilon) {
			return Eigen::Vector2f(-1, -1);
		}
#if 0
		Vector3f tmp = ray.origin() + tmin * ray.direction();
		qDebug() << " Intersection at (" << tmp.x() << "," << tmp.y() << "," << tmp.z() << ")"
			<< ", origin=(" << ray.origin().x() << "," << ray.origin().y() << "," << ray.origin().z() << ")";
#endif
		return Eigen::Vector2f(tmin, tmax);
	}
	return Eigen::Vector2f(-1, -1);
}

Eigen::Vector2f Raytracer::rayBoxIntersection(const Detector::Ray_t& ray) const
{
	return rayBoxIntersection(ray, volume_->getBoundingBox());
}

float Raytracer::voxelTraversal(const Eigen::VectorXf& vol) const
{
	//Implement "A fast voxel traversal algorithm for ray tracing"

	//variables
	int x = startX;
	int y = startY;
	int z = startZ;
	float maxX = tMaxX;
	float maxY = tMaxY;
	float maxZ = tMaxZ;

	//main loop
	float result = 0;
	do {
		result += vol[volume_->getVoxelIndex(x, y, z)];
		if (maxX < maxY) {
			if (maxX < maxZ) {
				x = x + stepX;
				if (x == justOutX)
					break; /* outside grid */
				maxX = maxX + tDeltaX;
			}
			else {
				z = z + stepZ;
				if (z == justOutZ)
					break;
				maxZ = maxZ + tDeltaZ;
			}
		}
		else {
			if (maxY < maxZ) {
				y = y + stepY;
				if (y == justOutY)
					break;
				maxY = maxY + tDeltaY;
			}
			else {
				z = z + stepZ;
				if (z == justOutZ)
					break;
				maxZ = maxZ + tDeltaZ;
			}
		}	
	} while (true);

	return result;
}

void Raytracer::voxelTraversalBackwards(float val, Eigen::VectorXf& vol) const
{
	//Implement "A fast voxel traversal algorithm for ray tracing", backward projection

	//variables
	int x = startX;
	int y = startY;
	int z = startZ;
	float maxX = tMaxX;
	float maxY = tMaxY;
	float maxZ = tMaxZ;

	//main loop
	do {
		vol[volume_->getVoxelIndex(x, y, z)] += val;
		if (maxX < maxY) {
			if (maxX < maxZ) {
				x = x + stepX;
				if (x == justOutX)
					break; /* outside grid */
				maxX = maxX + tDeltaX;
			}
			else {
				z = z + stepZ;
				if (z == justOutZ)
					break;
				maxZ = maxZ + tDeltaZ;
			}
		}
		else {
			if (maxY < maxZ) {
				y = y + stepY;
				if (y == justOutY)
					break;
				maxY = maxY + tDeltaY;
			}
			else {
				z = z + stepZ;
				if (z == justOutZ)
					break;
				maxZ = maxZ + tDeltaZ;
			}
		}
	} while (true);
}

void Raytracer::setupTraversal(const Detector::Ray_t& ray, float entry, float exit)
{
	const AlignedBox3f& box = volume_->getBoundingBox();
	const Vector3i& resolution = volume_->getNumVoxels();
	const Vector3f resolutionf(resolution.x(), resolution.y(), resolution.z());

	//initialization
	Vector3f start = ray.origin() + entry * ray.direction();
	Vector3f entryCell = (start - box.min()).array() / (box.max() - box.min()).array() * resolutionf.array();
	Vector3f end = ray.origin() + exit * ray.direction();
	Vector3f exitCell = (end - box.min()).array() / (box.max() - box.min()).array() * resolutionf.array();

	startX = clamp((int)entryCell.x(), 0, resolution.x() - 1);
	startY = clamp((int)entryCell.y(), 0, resolution.y() - 1);
	startZ = clamp((int)entryCell.z(), 0, resolution.z() - 1);
	endX = clamp((int)exitCell.x(), 0, resolution.x() - 1);
	endY = clamp((int)exitCell.y(), 0, resolution.y() - 1);
	endZ = clamp((int)exitCell.z(), 0, resolution.z() - 1);

	stepX = sgn(ray.direction().x());
	stepY = sgn(ray.direction().y());
	stepZ = sgn(ray.direction().z());

	justOutX = endX + stepX;
	justOutY = endY + stepY;
	justOutZ = endZ + stepZ;

	tDeltaX = fabs(1.0 / (exitCell.x() - entryCell.x()));
	tDeltaY = fabs(1.0 / (exitCell.y() - entryCell.y()));
	tDeltaZ = fabs(1.0 / (exitCell.z() - entryCell.z()));

	tMaxX = tDeltaX * (1 - frac(entryCell.x() / 1.0));
	tMaxY = tDeltaY * (1 - frac(entryCell.y() / 1.0));
	tMaxZ = tDeltaZ * (1 - frac(entryCell.z() / 1.0));
}
