#include "Tomograph.h"
#include<Eigen/Dense>
#include <Eigen/Geometry>
#include<vector>


Tomograph::Tomograph(const Eigen::AlignedBox2f& v, float s2dd, float dw, size_t N) : volume(v), s2dd(s2dd), detector(dw), N(N){
  if(!volumeFit())
      throw std::invalid_argument("The volume is too big for the gantry");    //if v too big

  Eigen::Vector2f mid = getMiddlePoint();
  setCenter(mid);
  setAngle(0);
}

bool Tomograph::volumeFit(){
  return s2dd >= volume.diagonal().norm();
}

Eigen::Vector2f Tomograph::getMiddlePoint() {
    Eigen::Vector2f bottomLeft = volume.corner(Eigen::AlignedBox2f::CornerType::BottomLeft);
    Eigen::Vector2f topRight = volume.corner(Eigen::AlignedBox2f::CornerType::TopRight);
    float coordinateX = (bottomLeft[0] + topRight[0])/2;
    float coordinateY = (bottomLeft[1] + topRight[1])/2;
    Eigen::Vector2f mid(coordinateX,coordinateY);
    return mid;
}

void Tomograph::setCenter(const Eigen::Vector2f& c){
    rotationCenter = c;
    alignCamera();
    alignDetector();
}

void Tomograph::setAngle(float alpha){
    angle = alpha;
}

Eigen::Vector2f Tomograph::getSourcePosition(){
    return doTransformation(cameraCenter);
}

Eigen::Vector2f Tomograph::getDetectorCenter(){
  return doTransformation(detectorCenter);
}

Eigen::Vector2f Tomograph::getDetectorNormal(){
  Eigen::Vector2f center = getDetectorCenter();
  Eigen::Vector2f camera = getSourcePosition();

  float x =  center[0]-camera[0];
  float y =  center[1]-camera[1];
  Eigen::Vector2f normal(x,y);

  float norm = normal.norm();
  float coordinateX = -normal[0] / norm;
  float coordinateY = -normal[1] / norm;
  Eigen::Vector2f detectorNormal(coordinateX,coordinateY);
  return detectorNormal;
}

Eigen::Vector2f Tomograph::getDetectorLeft(){
  return doTransformation(detectorLeft);
}

Eigen::Vector2f Tomograph::getDetectorRight(){
  return doTransformation(detectorRight);
}

Eigen::Vector2f Tomograph::doTransformation(Eigen::Vector2f vector) {
    Eigen::Rotation2D<float> rotation(angle);
    Eigen::Transform<float,2,Eigen::Affine> transform(rotation);
    Eigen::Vector2f rot = vector - rotationCenter;
    rot = rotation.toRotationMatrix()*rot;
    rot = rot + rotationCenter;
    return rot;
}

void Tomograph::alignCamera(){
    float cameraX = rotationCenter[0] - (s2dd/2);
    float cameraY = rotationCenter[1];
    Eigen::Vector2f cameraCent(cameraX,cameraY);
    cameraCenter = cameraCent;
}

void Tomograph::alignDetector(){
  float detectorX = rotationCenter[0] + (s2dd/2);
  float detectorY = rotationCenter[1];
  detectorCenter[0] = detectorX;
  detectorCenter[1] = detectorY;
  //change position of Left
  detectorLeft[0] = detectorX;
  detectorLeft[1] = detectorY + (detector/2);
  //change position of Right
  detectorRight[0] = detectorX;
  detectorRight[1] = detectorY - (detector/2);
}

std::vector<Eigen::ParametrizedLine<float, 2>> Tomograph::getProjectionRays(){
  std::vector<Eigen::ParametrizedLine<float, 2>> vecRays;
  if (N > 0) {
      if (N==1) {
        //go through the middle of the detector
          Eigen::ParametrizedLine<float, 2> line =
            Eigen::ParametrizedLine<float,2>::Through
              (getSourcePosition(),getDetectorCenter());
          vecRays.push_back(line);
      } else {
          //divide the detector into equal length spaces
          Eigen::Vector2f detectorL = getDetectorLeft();
          Eigen::Vector2f detectorR = getDetectorRight();
          float changeX = (detectorR[0] - detectorL[0])/N;
          float changeY = (detectorR[1] - detectorL[1])/N;

          float pointX = (changeX/2) + detectorL[0];
          float pointY = (changeY/2) + detectorL[1];

            //first ray
          Eigen::Vector2f startLine(pointX,pointY);
          Eigen::ParametrizedLine<float, 2> line =
                Eigen::ParametrizedLine<float, 2>::Through(getSourcePosition(), startLine);
          vecRays.push_back(line);
          for (size_t i = 1; i < N; i++) {
              pointX += changeX;
              pointY += changeY;

              Eigen::Vector2f rayLine(pointX ,pointY);
              line = Eigen::ParametrizedLine<float, 2>::Through(getSourcePosition(),rayLine);
              vecRays.push_back(line);
          }
        }
  }
  return vecRays;
}
