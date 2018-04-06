#include <math/Volume.h>

#include <stdexcept>

Volume::Volume(const Eigen::Vector3f& lowerLeft, const Eigen::Vector3f& upperRight, const Eigen::Vector3f& sp, const Eigen::Vector3i& numV)
	: boundingBox_(lowerLeft, upperRight)
	, spacing_(sp)
	, numVoxels_(numV)
{
}

Eigen::AlignedBox3f Volume::getBoundingBox() const
{
	return boundingBox_;
}

Eigen::Vector3f Volume::getSpacing() const
{
	return spacing_;
}

Eigen::Vector3i Volume::getNumVoxels() const
{
	return numVoxels_;
}

Eigen::VectorXf Volume::getContent() const
{
	return content_;
}

Eigen::VectorXf& Volume::getContent()
{
	return content_;
}

void Volume::setContent(const Eigen::VectorXf& c)
{
#if defined(DEBUG) || defined(_DEBUG)
	if (numVoxels_.x() * numVoxels_.y() * numVoxels_.z() != c.size())
	{
		throw std::invalid_argument("content has the wrong size");
	}
#endif
	content_ = c;
}

size_t Volume::getVoxelIndex(int x, int y, int z) const
{
#if defined(DEBUG) || defined(_DEBUG)
	if (x<0 || x >= numVoxels_.x() || y<0 || y >= numVoxels_.y() || z<0 || z >= numVoxels_.z()) {
		throw std::out_of_range("position is out of bounds");
	}
#endif
	return x + numVoxels_.x() * (y + numVoxels_.y() * z);
}

float Volume::getVoxel(int x, int y, int z) const
{

	return content_[getVoxelIndex(x, y, z)];
}

float Volume::getVoxelInterpolated(const Eigen::Vector3f& pos)
{
	int x = std::min((int)pos.x(), numVoxels_.x() - 2);
	int y = std::min((int)pos.y(), numVoxels_.x() - 2);
	int z = std::min((int)pos.z(), numVoxels_.x() - 2);
	float fx = pos.x() - x;
	float fy = pos.y() - y;
	float fz = pos.z() - z;
	Eigen::Vector4f data1(
		getVoxel(x, y, z),
		getVoxel(x+1, y, z),
		getVoxel(x, y+1, z),
		getVoxel(x+1,y+1,z)
	);
	Eigen::Vector4f data2(
		getVoxel(x, y, z+1),
		getVoxel(x+1, y, z+1),
		getVoxel(x, y+1, z+1),
		getVoxel(x+1,y+1,z+1)
	);
	Eigen::Vector4f i1 = (1 - fz) * data1 + fz * data2;
	Eigen::Vector2f i2 = (1 - fy) * i1.block(0, 0, 2, 1) + fy * i1.block(2, 0, 2, 1);
	float i3 = (1 - fx) * i2(0) + fx * i2(1);
	return i3;
}

float& Volume::accessVoxel(int x, int y, int z)
{
#if defined(DEBUG) || defined(_DEBUG)
	if (x<0 || x >= numVoxels_.x() || y<0 || y >= numVoxels_.y() || z<0 || z >= numVoxels_.z()) {
		throw std::out_of_range("position is out of bounds");
	}
#endif
	return content_[getVoxelIndex(x, y, z)];
}

void Volume::setVoxel(int x, int y, int z, float value)
{
#if defined(DEBUG) || defined(_DEBUG)
	if (x<0 || x >= numVoxels_.x() || y<0 || y >= numVoxels_.y() || z<0 || z >= numVoxels_.z())
	{
		throw std::out_of_range("position is out of bounds");
	}
#endif
	content_[getVoxelIndex(x, y, z)] = value;
}
