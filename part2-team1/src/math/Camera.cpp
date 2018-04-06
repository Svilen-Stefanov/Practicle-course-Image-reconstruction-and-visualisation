#include <math/Camera.h>

const float Camera::TRANSLATION_FACTOR = 0.05f;

Camera::Camera() 
:  perspectiveProj_(true)
 , back_(0.0001f)
 , front_(1000)
 , fov_(45)
 , rotationXRad_(45 * M_PI/180) 
 , rotationYRad_(45 * M_PI/180)
 , translationZ_(0)
{	

}

Camera::~Camera() {
	
}

void Camera::setPerspectiveProject(bool b) {
	perspectiveProj_ = b;
	updateFinalCameraMatrix();
}

Eigen::Matrix4f Camera::getPersProjectionMatrix() {
    Eigen::Matrix4f persprojMatrix;
    if (screenSize_.x() != 0 && screenSize_.y() != 0) {
    	Eigen::Vector2f ssf(screenSize_.x(),screenSize_.y());
    	float s1 = 1 / tan(fov_ * 0.5 * M_PI / 180);
    	float s2 = s1*(ssf.x()/ssf.y());
    	persprojMatrix<< s1, 0, 0, 0,
                    0, s2, 0, 0,
                    0, 0, -1*(back_ + front_)/ (back_ - front_), -2*(back_*front_)/ (back_ - front_),
    				0, 0, -1, 0;
        //std::cout << "pers project: " << persprojMatrix << std::endl;
    }
	return persprojMatrix;
}

Eigen::Matrix4f Camera::getOrthogonalProjectionMatrix() {
	Eigen::Matrix4f orthogProj;
    if (screenSize_.x() != 0 && screenSize_.y() != 0) {
        Eigen::Vector2f ssf(screenSize_.x(),screenSize_.y());
    	float s2 = ssf.x()/ssf.y();
		orthogProj << 1, 0, 0, 0,
				0 ,s2, 0 ,0,
				0, 0, -2/(back_-front_), -1*(back_ + front_) /(back_-front_),
				0, 0, 0, 1;
        //std::cout << "orthogonal project: " << orthogProj << std::endl;
	}
	return orthogProj;
}

//update the camera back to the default camera values
void Camera::setDefaultCameraValues() {
	perspectiveProj_ = true;
	translationZ_ = 0;
	updateFinalCameraMatrix();
}

void Camera::setScreenSize(const Eigen::Vector2i screenSize)
{
	screenSize_ = screenSize;
	updateFinalCameraMatrix();
}

Eigen::Vector3f Camera::getWorldCoordinates(float screenX, float screenY, float depth) const
{
	Eigen::Vector4f v(
		screenX / screenSize_.x() * 2 - 1, //the center is at (0,0)
		screenY / screenSize_.y() * 2 - 1, //and the screen extends from (-1,-1) to (1,1)
		depth * 2 - 1,
		1
	);
	v = invViewProjectionMatrix_ * v;
	return Eigen::Vector3f(v.x() / v.w(), v.y() / v.w(), v.z() / v.w());
}

Eigen::Vector3f Camera::getScreenCoordinates(const Eigen::Vector3f& worldCoordinates) const
{
	Eigen::Vector4f v(worldCoordinates.x(), worldCoordinates.y(), worldCoordinates.z(), 1.0f);
	v = viewProjectionMatrix_ * v;
	return Eigen::Vector3f (
		((v.x() / v.w()) + 1) / 2.0f * screenSize_.x(),
		((v.y() / v.w()) + 1) / 2.0f * screenSize_.y(),
		((v.z() / v.w()) + 1) / 2.0f
		);
}

void Camera::updateFinalCameraMatrix() {
	//final camera view projection matrix computation
    Eigen::AngleAxis<float> rotX(rotationXRad_, Eigen::Vector3f(0,1,0));
    Eigen::AngleAxis<float> rotY(rotationYRad_, Eigen::Vector3f(1,0,0));
	Eigen::Translation3f baseTranslation(-boundingBox_.center());
    
	//get base transformation matrix
    if (perspectiveProj_) { 
		//perspective projection
        perspectiveProjectionMatrix_ = getPersProjectionMatrix();
		float translation = translationZ_ - boundingBox_.diagonal().norm()*2;
		Eigen::Transform<float, 3, Eigen::Affine> tt = Eigen::Translation3f(0, 0, translation) * (rotY * rotX * baseTranslation);
    	viewProjectionMatrix_ = perspectiveProjectionMatrix_ * tt.matrix();
        invViewProjectionMatrix_ = viewProjectionMatrix_.inverse();
	} else { 
		//orthogonal projection
		orthogonalProjectionMatrix_ = getOrthogonalProjectionMatrix();
		float scaling = pow(2, translationZ_) / (boundingBox_.diagonal().norm());
		Eigen::Transform<float, 3, Eigen::Affine> tt = rotY * rotX * Eigen::Scaling(scaling) *  baseTranslation;
        viewProjectionMatrix_ = orthogonalProjectionMatrix_ * tt.matrix();
        invViewProjectionMatrix_ = viewProjectionMatrix_.inverse();
	}
}

void Camera::setTranslation(int t) {
	translationZ_ += t * TRANSLATION_FACTOR;
	updateFinalCameraMatrix();
}

void Camera::setRotationX(float a) {
	rotationXRad_ = a;
	updateFinalCameraMatrix();
}

void Camera::setRotationY(float a) {
	rotationYRad_ = a;
	updateFinalCameraMatrix();
}

void Camera::setBoundingBox(const Eigen::AlignedBox3f& box)
{
	boundingBox_ = box;
}

Camera Camera::createTestCamera(const Eigen::Vector2i& screenSize)
{
	//matrices exported from another project
	Eigen::Matrix4f viewprojMat;

	viewprojMat << // a slight rotation + translation. Box (-1,-1,-1) - (1,1,1) is in the center
		1.8095572, 0.06319112, -5.0589257E-9, 2.1905793E-8,
		-0.07296682, 2.0894966, 1.2071066, 0.0,
		-0.017484682, 0.50069577, -0.8677592, 3.0080078,
		-0.017449748, 0.49969536, -0.86602545, 5.0;
	
#if 1
	viewprojMat = viewprojMat * Eigen::Vector4f(5, 5, 5, 1).asDiagonal(); //transforms it nearer
#endif

	Camera cam;
	cam.viewProjectionMatrix_ = viewprojMat;
	cam.invViewProjectionMatrix_ = cam.viewProjectionMatrix_.inverse();
	cam.screenSize_ = screenSize;
	return cam;
}
