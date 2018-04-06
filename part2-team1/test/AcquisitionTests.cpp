#include "catch.hpp"
#include "TestUtils.h"

#include <string>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <math/Volume.h>
#include <io/EDFhandler.h>
#include <math/Detector.h>
#include <math/Raytracer.h>

using namespace std;
using namespace Eigen;

const string EDF_FILE = "resources/lshape.edf";

TEST_CASE("loader", "[acquisition]")
{
	Volume_ptr vol = EDFHandler::read(EDF_FILE);
	REQUIRE(vol->getNumVoxels() == Eigen::Vector3i(10, 10, 10));
	REQUIRE(vol->getSpacing().isApprox(Eigen::Vector3f(0.015, 0.015, 0.025)));
	REQUIRE(vol->getBoundingBox().min() == Eigen::Vector3f(0, 0, 0));
	REQUIRE(vol->getBoundingBox().max().isApprox(Eigen::Vector3f(0.15, 0.15, 0.25)));
	for (int x = 0; x < 10; ++x) {
		for (int y = 0; y < 10; ++y) {
			for (int z = 0; z < 10; ++z) {
				//value can be accessed
				REQUIRE_NOTHROW(vol->getVoxel(x, y, z));
			}
		}
	}
}

void testDetector(Detector detector)
{
	Quaternionf rot = detector.getRotation();
	Vector3f source = rot._transformVector(Vector3f(-detector.getDistance(), 0, 0)) + detector.getCenter();
	Vector3f target = rot._transformVector(Vector3f(+detector.getDistance(), 0, 0)) + detector.getCenter();
	Hyperplane<float, 3> plane((target - source).normalized(), target);

	INFO("Detector: source=(" << source.x() << "," << source.y() << "," << source.z()
		<< "), target=(" << target.x() << "," << target.y() << "," << target.z() 
		<< "), rotation=(" << rot.w() << "," << rot.x() << "," << rot.y() << "," << rot.z() << ")");

	{
		INFO("Resolution: 1x1");
		detector.setResolution(Vector2i(1, 1));
		auto rays = detector.computeRays();
		REQUIRE(rays.size() == 1);
		REQUIRE(rays[0].distance(source) == Approx(0.0));
		REQUIRE(rays[0].distance(target) == Approx(0.0));
		REQUIRE(rays[0].intersection(plane) == Approx(0.0));
	}

	{
		INFO("Resolution: 5x5");
		detector.setResolution(Vector2i(5, 5));
		auto rays = detector.computeRays();
		REQUIRE(rays.size() == 5 * 5);
		for (int i = 0; i < 25; ++i) {
			INFO("ray " << i << ", dir=(" << rays[i].direction().x() << "," << rays[i].direction().y() << "," << rays[i].direction().z() << ")");
			REQUIRE(rays[i].distance(source) == Approx(0.0).epsilon(0.001));
			REQUIRE(rays[i].intersection(plane) == Approx(0.0).epsilon(0.001));
		}
		REQUIRE(rays[12].distance(target) == Approx(0.0).epsilon(0.001));
	}

	{
		INFO("Resolution: 8x8");
		detector.setResolution(Vector2i(8, 8));
		auto rays = detector.computeRays();
		REQUIRE(rays.size() == 64);
		for (int i = 0; i < 64; ++i) {
			INFO("ray " << i << ", dir=(" << rays[i].direction().x() << "," << rays[i].direction().y() << "," << rays[i].direction().z() << ")");
			REQUIRE(rays[i].distance(source) == Approx(0.0).epsilon(0.001));
			REQUIRE(rays[i].intersection(plane) == Approx(0.0).epsilon(0.001));
			REQUIRE_FALSE(rays[i].distance(target) == Approx(0.0));
		}
	}
}
TEST_CASE("detector", "[acquisition][!hide]")
//TEST_CASE("detector", "[acquisition]")
{
	RandomUtils rand;

	for (int i = 0; i < 50; ++i)
	{
		Detector detector;
		detector.setCenter(Vector3f(rand.nextFloat(-50, 50), rand.nextFloat(-50, 50), rand.nextFloat(-50, 50)));
		detector.setDistance(rand.nextFloat(0.01, 2));
		float s = rand.nextFloat(0.001, 0.5);
		detector.setSize(Vector2f(s, s));
		detector.setRotation(rand.nextRandomRotation<float>());
		testDetector(detector);
	}
}

TEST_CASE("raytracer-utilities", "[acquisition]")
{
	REQUIRE(Raytracer::clamp(1.5, 1.0, 7.0) == 1.5);
	REQUIRE(Raytracer::clamp(0.5, 1.3, 7.0) == 1.3);
	REQUIRE(Raytracer::clamp(12.5, -0.7, 4.6) == 4.6);

	REQUIRE(Raytracer::frac(0) == Approx(0));
	REQUIRE(Raytracer::frac(1) == Approx(0));
	REQUIRE(Raytracer::frac(2) == Approx(0));
	REQUIRE(Raytracer::frac(-1) == Approx(0));
	REQUIRE(Raytracer::frac(0.5) == Approx(0.5));
	REQUIRE(Raytracer::frac(42.5) == Approx(0.5));
	REQUIRE(Raytracer::frac(1.7) == Approx(0.7));
	REQUIRE(Raytracer::frac(-0.5) == Approx(0.5));
	REQUIRE(Raytracer::frac(-0.7) == Approx(0.3));
	REQUIRE(Raytracer::frac(-1.2) == Approx(0.8));
}

float callVoxelTraversal(Volume_ptr vol, const Detector::Ray_t& ray, float start, float end)
{
	Raytracer t(vol, ray);
	t.setupTraversal(ray, start, end);
	return t.voxelTraversal(vol->getContent());
}

TEST_CASE("raytracer-voxel", "[acquisition]")
{
	//create volume
	Vector3f min(-0.2, -0.3, -0.2);
	Vector3f max(0.2, 0.3, 0.2);
	Vector3i resolution(2, 2, 2);
	Vector3f sp(0.2, 0.3, 0.2);  //(max-min)/resolution
	Volume_ptr vol = make_shared<Volume>(min, max, sp, resolution);

	//fill with powers of two
	VectorXf content(8);
	for (int i = 0; i < 8; ++i) content[i] = 1 << i;
	vol->setContent(content);


	//straight lines
	
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.1, -1.3, -0.1), Vector3f(0, 1, 0)), 1, 1.6) == Approx(5));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(0.06, -1.3, -0.1), Vector3f(0, 1, 0)), 1, 1.6) == Approx(10));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.1, 1.5, -0.1), Vector3f(0, -1, 0)), 1.2, 1.8) == Approx(5));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(0.06, 1.5, -0.1), Vector3f(0, -1, 0)), 1.2, 1.8) == Approx(10));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.1, -1.3, 0.13), Vector3f(0, 1, 0)), 1, 1.6) == Approx(80));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(0.06, -1.3, 0.03), Vector3f(0, 1, 0)), 1, 1.6) == Approx(160));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.1, 1.5, 0.07), Vector3f(0, -1, 0)), 1.2, 1.8) == Approx(80));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(0.06, 1.5, 0.09), Vector3f(0, -1, 0)), 1.2, 1.8) == Approx(160));

	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.5, -0.16, -0.1), Vector3f(1, 0, 0)), 0.3, 0.7) == Approx(3));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.5, 0.03, -0.1), Vector3f(1, 0, 0)), 0.3, 0.7) == Approx(12));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.5, -0.16, -0.1), Vector3f(1, 0, 0)), 0.3, 0.7) == Approx(3));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.5, 0.03, -0.1), Vector3f(1, 0, 0)), 0.3, 0.7) == Approx(12));

	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.06, -0.16, -0.8), Vector3f(0, 0, 1)), 0.6, 1.0) == Approx(17));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.06, 0.09, -0.8), Vector3f(0, 0, 1)), 0.6, 1.0) == Approx(68));

	//diagonal line
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(-0.25, -0.3, -0.1), Vector3f(1, 1, 0).normalized()), 0.05*sqrt(2), 0.45*sqrt(2)) == Approx(11));
	REQUIRE(callVoxelTraversal(vol, Detector::Ray_t(Vector3f(0.24888, 0.3, 0.1), Vector3f(-1, -1, -0.00001).normalized()), 0.05*sqrt(2), 0.45*sqrt(2)) == Approx(16+64+128));
}