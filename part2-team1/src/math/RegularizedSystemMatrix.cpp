#include <math/RegularizedSystemMatrix.h>

RegularizedSystemMatrix::RegularizedSystemMatrix(Volume_ptr volume, AcquisitionPoses acquisitionPoses, float lambda)
	: SystemMatrix(volume, acquisitionPoses), lambda_(lambda)
{}

void RegularizedSystemMatrix::apply(const Eigen::VectorXf& x, Eigen::VectorXf& b)
{
	//computes (A'A + \lambda I)x = b
	//      == (A'A)x + (\lambda I)x
	//      == super.apply() + \lambda*x

	SystemMatrix::apply(x, b);
	b += lambda_ * x;
}
