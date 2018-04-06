#ifndef VOLUME_H
#define VOLUME_H

#include <memory>
#include <Eigen/Geometry>
#include <string>

/**
 * \brief Represents the volume.
 * It defines the bounding box, the resolution and the resulting voxel spacing.
 * The content is linearized in a vector: index = x + width*(y + height*z)
 */
class Volume {
public:
	Volume(const Eigen::Vector3f& lowerLeft, const Eigen::Vector3f& upperRight, const Eigen::Vector3f& sp, const Eigen::Vector3i& numV);

    Eigen::AlignedBox3f getBoundingBox() const;
    Eigen::Vector3f getSpacing() const;
    Eigen::Vector3i getNumVoxels() const;

    Eigen::VectorXf getContent() const;
	Eigen::VectorXf& getContent();
    void setContent(const Eigen::VectorXf& c);

	size_t getVoxelIndex(int x, int y, int z) const;
	inline size_t getVoxelIndex(const Eigen::Vector3i& pos) const { return getVoxelIndex(pos.x(), pos.y(), pos.z()); }

	float getVoxel(int x, int y, int z) const;
	float getVoxel(const Eigen::Vector3i& pos) const { return getVoxel(pos.x(), pos.y(), pos.z()); }

	/**
	 * \brief Returns the voxel value at the specified position using trilinear interpolation
	 * \param pos the position
	 * \return the voxel value
	 */
	float getVoxelInterpolated(const Eigen::Vector3f& pos);

	float& accessVoxel(int x, int y, int z);

	void setVoxel(int x, int y, int z, float value);
	void setVoxel(const Eigen::Vector3i& pos, float value) { setVoxel(pos.x(), pos.y(), pos.z(), value); }

private:
    Eigen::AlignedBox3f boundingBox_;
    Eigen::Vector3f spacing_;
	Eigen::Vector3i numVoxels_;

    Eigen::VectorXf content_;
};

typedef std::shared_ptr<Volume> Volume_ptr;

#endif // VOLUME_H
