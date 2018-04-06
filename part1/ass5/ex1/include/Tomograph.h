#ifndef TOMOGRAPH_H
#define TOMOGRAPH_H
#include<iostream>
#include<vector>
#include<Eigen/Dense>

class Tomograph {

private:
  Eigen::Vector2f cameraCenter;
  Eigen::Vector2f rotationCenter;
  Eigen::Vector2f detectorCenter;
  Eigen::Vector2f detectorLeft;
  Eigen::Vector2f detectorRight;
  Eigen::AlignedBox2f volume;
  float angle;

  float detector;
  float s2dd;
  bool volumeFit();
  size_t N;
  void alignCamera();       //set the position of the camera on the same line as rotation center
  void alignDetector();     //set the position of the detector on the same line as rotation center


public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  Tomograph(const Eigen::AlignedBox2f&, float, float, size_t);
  void setCenter(const Eigen::Vector2f&);
  void setAngle(float);
  Eigen::Vector2f getMiddlePoint();
  Eigen::Vector2f getSourcePosition();
  Eigen::Vector2f getDetectorCenter();
  Eigen::Vector2f getDetectorNormal();
  Eigen::Vector2f getDetectorLeft();
  Eigen::Vector2f getDetectorRight();
  Eigen::Vector2f doTransformation(Eigen::Vector2f vector);
  std::vector<Eigen::ParametrizedLine<float, 2>> getProjectionRays();

};

#endif
