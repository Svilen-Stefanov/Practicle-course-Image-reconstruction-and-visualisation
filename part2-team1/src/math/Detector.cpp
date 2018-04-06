#include <math/Detector.h>

#include <stdexcept>
#include <QDebug>

using namespace std;
using namespace Eigen;

Detector::Detector(
	const Vector2f& size,
	const Vector2i& resolution,
	const Vector3f& center,
	float distance,
	const Quaternionf& rotation)
{
	setSize(size);
	setResolution(resolution);
	setCenter(center);
	setDistance(distance);
	setRotation(rotation);
}

void Detector::setSize(const Vector2f& size)
{
	if (size.x() <= 0 || size.y() <= 0)
	{
		throw invalid_argument("invalid size");
	}
	size_ = size;
}

void Detector::setResolution(const Vector2i& resolution)
{
	if (resolution.x() <= 0 || resolution.y() <= 0)
	{
		throw invalid_argument("invalid resolution");
	}
	resolution_ = resolution;
}

void Detector::setCenter(const Vector3f& center)
{
	center_ = center;
}

void Detector::setDistance(float distance)
{
	if (distance <= 0)
	{
		throw invalid_argument("invalid distance");
	}
	distance_ = distance;
}

void Detector::setRotation(const Quaternionf& rotation)
{
	rotation_ = rotation;
}

vector<Detector::Ray_t> Detector::computeRays() const
{
	vector<Ray_t> result(resolution_.x() * resolution_.y());

	Vector3f source = Vector3f(-distance_, 0, 0);
	const Vector3f center = Vector3f(distance_, 0, 0);
	const Vector3f tangent1 = Vector3f(0, 1, 0);
	const Vector3f tangent2 = Vector3f(0, 0, 1);
	Vector2f step = Vector2f(size_.x() / resolution_.x(), size_.y() / resolution_.y());

	//transform source
	Matrix3f rotMat = rotation_.toRotationMatrix();
	source = rotMat * source;
	source += center_;

#if 0
	Vector3f tmp = (rotMat * center) + center_;
	qDebug() << " ray source=(" << source.x() << "," << source.y() << "," << source.z() << "), center=("
		<< tmp.x() << "," << tmp.y() << "," << tmp.z() << ")";
#endif

	for (int x = 0; x < resolution_.x(); ++x)
	{
		for (int y = 0; y < resolution_.y(); ++y)
		{
			Vector3f target = center
				+ tangent1 * ((x + 0.5 - resolution_.x()/2.0) * step.x())
				+ tangent2 * ((y + 0.5 - resolution_.y()/2.0) * step.y());
			//transform it
			target = (rotMat * target) + center_;
#if 0
			qDebug() << " target=(" << target.x() << "," << target.y() << "," << target.z() << ")";
#endif
			//create ray
			Vector3f dir = (source - target).normalized();
			//check that direction does not contain zero
			dir = checkForZero(dir);
			result[x + resolution_.x() * y] = Ray_t(target, dir);
		}
	}

	return result;
}

 Vector3f Detector::checkForZero(Vector3f vec) {
	Vector3f newVec(0.00000000000001,0.00000000000001,0.00000000000001);
	for (int i = 0; i < newVec.size(); i++) {
		if (vec[i] != 0) {
			newVec[i] = vec[i];
        }
    }
	return newVec;
}
