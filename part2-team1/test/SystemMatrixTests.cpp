#include "catch.hpp"

#include <math/SystemMatrix.h>
#include <io/EDFhandler.h>
#include <math/Reconstruction.h>

SCENARIO("Test functionality of the system matrix") {
  Detector detector;
  detector.setCenter(Eigen::Vector3f(2.5, 1.75, 0.5));
  detector.setDistance(0.5);
  detector.setResolution(Eigen::Vector2i(2,2));
  float s = 0.5;
  detector.setSize(Eigen::Vector2f(s, s));

  Eigen::Vector3f min(1,1,0);
  Eigen::Vector3f max(2,2,2);
  Eigen::Vector3i resolution(2,2,1);
  Eigen::Vector3f sp(0.015, 0.015, 0.025);
  Volume_ptr vol = std::make_shared<Volume>(min,max,sp,resolution);

  AcquisitionPoses acquisitionPoses(vol->getBoundingBox());
  std::vector<AcquisitionPose> poses;

  AcquisitionPose pose(detector);
  //only one pose
  acquisitionPoses.getPoses().push_back(pose);
  SystemMatrix sys(vol, acquisitionPoses);

  Eigen::VectorXf vol_content(4);
  vol_content[0] = 1;
  vol_content[1] = 5;
  vol_content[2] = 7;
  vol_content[3] = 9;

  Eigen::VectorXf vec(4); //solution to mult, and input for multTrans
  vec[0] = 16;
  vec[1] = 16;
  vec[2] = 16;
  vec[3] = 16;

	GIVEN("test mult") {
    Eigen::VectorXf vecResult = sys.mult(vol_content);

    REQUIRE(vec[0] == Approx(vecResult[0]));
    REQUIRE(vec[1] == Approx(vecResult[1]));
    REQUIRE(vec[2] == Approx(vecResult[2]));
    REQUIRE(vec[3] == Approx(vecResult[3]));
	}

  GIVEN("test multTrans") {
    Eigen::VectorXf vecResult(4);
    vecResult.setZero();
    sys.multTrans(vec, vecResult);

    Eigen::VectorXf solution(4);
    solution[0] = 0;
    solution[1] = 0;
    solution[2] = 64; //4*16
    solution[3] = 64;
    REQUIRE(solution[0] == Approx(vecResult[0]));
    REQUIRE(solution[1] == Approx(vecResult[1]));
    REQUIRE(solution[2] == Approx(vecResult[2]));
    REQUIRE(solution[3] == Approx(vecResult[3]));
  }

  /*
  * SECOND POSE
  * translated detector down
  */

  detector.setCenter(Eigen::Vector3f(2.5, 1.25, 0.5));
  AcquisitionPose pose2(detector);
  //only one pose
  acquisitionPoses.getPoses().pop_back();
  acquisitionPoses.getPoses().push_back(pose2);
  SystemMatrix sys2(vol, acquisitionPoses);

  Eigen::VectorXf vec2(4); //solution to mult after the translation, and input for multTrans
  vec2[0] = 6;
  vec2[1] = 6;
  vec2[2] = 6;
  vec2[3] = 6;

  GIVEN("test mult with translated detector") {
    Eigen::VectorXf vecResult = sys2.mult(vol_content);

    REQUIRE(vec2[0] == Approx(vecResult[0]));
    REQUIRE(vec2[1] == Approx(vecResult[1]));
    REQUIRE(vec2[2] == Approx(vecResult[2]));
    REQUIRE(vec2[3] == Approx(vecResult[3]));
  }

  GIVEN("test multTrans with translated detector") {
    Eigen::VectorXf vecResult(4);
    vecResult.setZero();
    sys2.multTrans(vec2, vecResult);

    Eigen::VectorXf solution(4);
    solution[0] = 24; //4*6
    solution[1] = 24;
    solution[2] = 0;
    solution[3] = 0;
    REQUIRE(solution[0] == Approx(vecResult[0]));
    REQUIRE(solution[1] == Approx(vecResult[1]));
    REQUIRE(solution[2] == Approx(vecResult[2]));
    REQUIRE(solution[3] == Approx(vecResult[3]));
  }
}

SCENARIO("Another test of the functionality of the system matrix") {
  Detector detector;
  detector.setCenter(Eigen::Vector3f(4.5, 1.25, 0.5));
  detector.setDistance(1);
  detector.setResolution(Eigen::Vector2i(2,3));
  detector.setSize(Eigen::Vector2f(0.3, 0.3));

  Eigen::Vector3f min(1,1,0);
  Eigen::Vector3f max(4,4,4);
  Eigen::Vector3i resolution(3, 3, 1);
  Eigen::Vector3f sp(0.015, 0.015, 0.025);
  Volume_ptr vol = std::make_shared<Volume>(min,max,sp,resolution);

  AcquisitionPoses acquisitionPoses(vol->getBoundingBox());
  std::vector<AcquisitionPose> poses;

  AcquisitionPose pose(detector);
  acquisitionPoses.getPoses().push_back(pose);
  SystemMatrix sys(vol, acquisitionPoses);

  Eigen::VectorXf vol_content(9);
  vol_content[0] = 10;
  vol_content[1] = 20;
  vol_content[2] = 30;
  vol_content[3] = 40;
  vol_content[4] = 50;
  vol_content[5] = 60;
  vol_content[6] = 70;
  vol_content[7] = 80;
  vol_content[8] = 90;

  Eigen::VectorXf vec(6);
  vec[0] = 60;
  vec[1] = 60;
  vec[2] = 60;
  vec[3] = 60;
  vec[4] = 60;
  vec[5] = 60;

	GIVEN("test mult") {
    Eigen::VectorXf vecResult = sys.mult(vol_content);

    REQUIRE(vec[0] == Approx(vecResult[0]));
    REQUIRE(vec[1] == Approx(vecResult[1]));
    REQUIRE(vec[2] == Approx(vecResult[2]));
    REQUIRE(vec[3] == Approx(vecResult[3]));
    REQUIRE(vec[4] == Approx(vecResult[4]));
    REQUIRE(vec[5] == Approx(vecResult[5]));
	}

	GIVEN("test multTrans") {
    Eigen::VectorXf vecResult(6);
    vecResult.setZero();
    sys.multTrans(vec, vecResult);

    Eigen::VectorXf solution(6);
    solution[0] = 360;
    solution[1] = 360;
    solution[2] = 360;
    solution[3] = 0;
    solution[4] = 0;
    solution[5] = 0;
    REQUIRE(solution[0] == Approx(vecResult[0]));
    REQUIRE(solution[1] == Approx(vecResult[1]));
    REQUIRE(solution[2] == Approx(vecResult[2]));
    REQUIRE(solution[3] == Approx(vecResult[3]));
    REQUIRE(solution[4] == Approx(vecResult[4]));
    REQUIRE(solution[5] == Approx(vecResult[5]));
	}
}

SCENARIO("Test if a real volume is the solution of the equation")
{
	Volume_ptr vol = EDFHandler::read("resources/lshape.edf");

	//build poses, copied from AcquisitionWidget
	AcquisitionPoses poses;
	int numberOfSlices = 1;
	float zBeginSlice = 0;
	float zEndSlice = 0;
	int radialCount = 2;
	Eigen::Vector2f size(0.4, 0.4);
	Eigen::Vector2i resolution(5, 5);
	float distance = 1;

	float anglesInDegrees = 360.0f / radialCount;
	float PI = 3.14159265358979323846;

	poses.getPoses().clear();
	float degreeValue;
	Eigen::Quaternionf rotation;
	Eigen::AngleAxisf angleAxis;
	Eigen::Vector3f center(0, 0, 0);
	Eigen::Vector3f axis = Eigen::Vector3f(0, 0, 1);
	if (vol != nullptr) center = vol->getBoundingBox().center();
	for (int j = 0; j < numberOfSlices; ++j) {
		double z = zBeginSlice + (j == 0) ? 0 : (j*(zEndSlice - zBeginSlice) / (double)(numberOfSlices - 1));
		for (int i = 0; i < radialCount; i++) {
			Detector d;
			d.setSize(size);
			d.setResolution(resolution);
			d.setDistance(distance);
			degreeValue = (((i)*anglesInDegrees) / 180.0) * PI;
			angleAxis = Eigen::AngleAxisf(degreeValue, axis);
			rotation = angleAxis;
			d.setRotation(rotation);
			d.setCenter(center + (float)z * axis);
			AcquisitionPose poseToAdd(d);
			poses.getPoses().push_back(poseToAdd);
		}
	}

	//compute pose output
	for (AcquisitionPose& p : poses.getPoses())
	{
		p.compute(vol);
	}

	//build reconstruction 
	Reconstruction r;
	r.setVolume(vol);
	r.setAcquisitionPoses(poses);
	r.setEpsilon(0.01f);
	r.setLambda(0.1f);
	r.setNoise(0);
	r.setUseRegularizer(false);
	SystemMatrix mat (vol, poses);
	Eigen::VectorXf rhsPrime = r.buildRhs();
	Eigen::VectorXf expectedSolution = vol->getContent();

	GIVEN("Test normal system matrix")
	{
		//test if the original problem creates the rhs
		Eigen::VectorXf actualRhsPrime = mat.mult(expectedSolution);

		Eigen::MatrixXf m(rhsPrime.size(), 2);
		for (int i = 0; i < rhsPrime.size(); ++i)
		{
			m(i, 0) = rhsPrime(i);
			m(i, 1) = actualRhsPrime(i);
		}
		INFO("rhs comparison:\n" << m);

		float resiual = (rhsPrime - actualRhsPrime).norm();
		REQUIRE(0.0 == Approx(resiual));
	}

	GIVEN("Test minimization")
	{
		//test if the original problem is a solution to the minimization
		Eigen::VectorXf rhs(vol->getNumVoxels().prod());
		rhs.setZero();
		mat.multTrans(rhsPrime, rhs);
		Eigen::VectorXf actualRhs(rhs.size());
		mat.apply(expectedSolution, actualRhs);

		Eigen::MatrixXf m(rhsPrime.size(), 2);
		for (int i = 0; i < rhsPrime.size(); ++i) {
			m(i, 0) = rhs(i);
			m(i, 1) = actualRhs(i);
		}
		INFO("volue comparison:\n" << m);

		float residual = (rhs - actualRhs).norm();
		REQUIRE(0.0 == Approx(residual));
	}
}