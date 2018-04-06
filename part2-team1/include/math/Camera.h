#ifndef CAMERA_H
#define CAMERA_H

#include <QtWidgets>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <vector>
/**
 * \brief The camera class.
 * The camera manages the view and projection matrices and frustum computations.
 * It provides a method to project a point on the screen plus the depth back into the world.
 * This is needed to compute the rays for the volume renderer.
 */
class Camera
{
private:
	//perspective projection true, false -> orthogonal projects
	bool perspectiveProj_;
	//the screen settings
	Eigen::Vector2i screenSize_;

	//the view-projection matrices
	Eigen::Matrix4f viewProjectionMatrix_;
	Eigen::Matrix4f invViewProjectionMatrix_;
	Eigen::Matrix4f perspectiveProjectionMatrix_;
	Eigen::Matrix4f orthogonalProjectionMatrix_;

	//rotation 
	float rotationXRad_;
	float rotationYRad_;

	//translation
	float translationZ_;
	static const float TRANSLATION_FACTOR;

	//far and near clipping planes
	float back_; //cannot use near and far because it causes a windows compile error
	float front_;
	float fov_;

	//the volume bounding box to get the center right
	Eigen::AlignedBox3f boundingBox_;

	void updateFinalCameraMatrix();
    Eigen::Matrix4f getPersProjectionMatrix(); //get projection matrix
    Eigen::Matrix4f getOrthogonalProjectionMatrix(); //get orthogonal 
public:
	Camera();
	~Camera();
	/**
	 * \brief Sets the screen size.
	 *        This is needed to compute the correct aspect ratio and screen transformation
	 * \param screenSize the screen size in pixels
	 */
	void setScreenSize(const Eigen::Vector2i screenSize);
	
	/**
	 * \brief Computes the world position from the specified screen coordinates.
	 * This is the inverse of getScreenCoordinates(const Eigen::Vector3f& worldCoordinates).
	 * \param screenX the screen position, between 0 and screenSize.x
	 * \param screenY the screen position, between 0 and screenSize.y
	 * \param depth the depth
	 * \return the vector in the world
	 */
	Eigen::Vector3f getWorldCoordinates(float screenX, float screenY, float depth) const;

	/**
	 * \brief Computes the screen position from the specified world coordinates.
	 * This is the inverse of getWorldCoordinates(float screenX, float screenY, float depth).
	 * \param worldCoordinates the world coordinates
	 * \return the screen coordinates (x, y, depth)
	 */
	Eigen::Vector3f getScreenCoordinates(const Eigen::Vector3f& worldCoordinates) const;

	/**
	 * \brief Creates a test camera, used for the volume renderer unless another camera is specified
	 * \return 
	 */
	static Camera createTestCamera(const Eigen::Vector2i& screenSize);

	//translations & rotation setters
	void setTranslation(int t); //really does an addition of vectors!
	void setRotationX(float); //sets the rotation angle for x in radians
	void setRotationY(float); //sets the rotation angle for y in radians
	void setBoundingBox(const Eigen::AlignedBox3f& box);

	void setDefaultCameraValues(); //returns to default camera values
	void setPerspectiveProject(bool); //boolean trigger for perspective projection

};

#endif