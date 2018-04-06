#include "catch.hpp"
#include "TestUtils.h"

#include <math/Volume.h>

using namespace Eigen;
using namespace std;

SCENARIO("Trilinear voxel interpolation")
{
	//create volume
	Vector3f min(-0.2, -0.3, -0.2);
	Vector3f max(0.2, 0.3, 0.2);
	Vector3i resolution(3, 2, 2);
	Vector3f sp(0.2, 0.3, 0.2);  //(max-min)/resolution
	Volume_ptr vol = make_shared<Volume>(min, max, sp, resolution);

	//fill with powers of two
	VectorXf content(12);
	for (int i = 0; i < 12; ++i) content[i] = 1 << i;
	vol->setContent(content);

	//check some interpolated values
	REQUIRE(vol->getVoxelInterpolated(Vector3f(0.5, 0.5, 0.5)) == Approx((1 + 2 + 8 + 16 + 64 + 128 + 512 + 1024) / 8.0));
}