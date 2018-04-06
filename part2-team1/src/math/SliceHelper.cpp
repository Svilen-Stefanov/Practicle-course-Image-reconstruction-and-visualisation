#include <math/SliceHelper.h>

#include <algorithm>
#include <assert.h>

using namespace Eigen;
using namespace std;

SliceHelper::SliceHelper(const AlignedBox3f& box)
	: box_(box), plane_(1, 0, 0, 0)
{
}

void SliceHelper::setSlice(const Vector3f normal, float distanceFromCenter)
{
	plane_.head<3>() = normal;
	plane_.w() = distanceFromCenter - box_.center().dot(normal);
}

vector<Vector3f> SliceHelper::computeIntersectionPolygon() const
{
	vector<Vector3f> points = calc_plane_aabb_intersection_points();
	sort_points(points);
	return points;
}

bool SliceHelper::intersectRay(const Vector3f& rayStart, const Vector3f& rayDirection, Vector3f& result) const
{
	float t, vd;
	if (ray_to_plane(rayStart, rayDirection, &t, &vd))
	{
		Vector3f p = rayStart + t * rayDirection;
		if (box_.contains(p))
		{
			result = p;
			return true;
		}
	}
	return false;
}

// Code taken from
// http://asawicki.info/news_1428_finding_polygon_of_plane-aabb_intersection.html

bool SliceHelper::ray_to_plane(const Vector3f& rayOrig, const Vector3f& rayDir, float* outT, float* outVD) const
{
	*outVD = plane_.head<3>().dot(rayDir);
	if (abs(*outVD) < numeric_limits<float>::epsilon())
		return false;
	*outT = -(plane_.head<3>().dot(rayOrig) + plane_.w()) / *outVD;
	return true;
}

vector<Vector3f> SliceHelper::calc_plane_aabb_intersection_points() const
{
	float vd, t;
	vector<Vector3f> out;

	//Test edges along X axis, pointing right
	Vector3f dir = Vector3f(box_.max().x() - box_.min().x(), 0, 0);
	Vector3f orig = box_.min();
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.min().x(), box_.max().y(), box_.min().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.min().x(), box_.min().y(), box_.max().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.min().x(), box_.max().y(), box_.max().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);

	//Test edges along Y axis, pointing up
	dir = Vector3f(0, box_.max().y() - box_.min().y(), 0);
	orig = Vector3f(box_.min().x(), box_.min().y(), box_.min().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.max().x(), box_.min().y(), box_.min().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.min().x(), box_.min().y(), box_.max().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.max().x(), box_.min().y(), box_.max().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);

	//Test edges along Z axis, pointing forward
	dir = Vector3f(0, 0, box_.max().z() - box_.min().z());
	orig = Vector3f(box_.min().x(), box_.min().y(), box_.min().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.max().x(), box_.min().y(), box_.min().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.min().x(), box_.max().y(), box_.min().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);
	orig = Vector3f(box_.max().x(), box_.max().y(), box_.min().z());
	if (ray_to_plane(orig, dir, &t, &vd) && t >= 0 && t < 1)
		out.push_back(orig + dir * t);

	assert(out.size() <= 6);
	return out;
}

void SliceHelper::sort_points(vector<Vector3f>& points) const
{
	if (points.empty()) return;
	Vector3f normal = plane_.head<3>();
	Vector3f origin = points[0];
	sort(points.begin(), points.end(), [&](const Vector3f& lhs, const Vector3f& rhs)
	{
		Vector3f v = (lhs - origin).cross(rhs - origin);
		return v.dot(normal) < 0;
	});
}
