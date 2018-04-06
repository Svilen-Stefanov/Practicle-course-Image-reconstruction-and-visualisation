#ifndef REGULARIZED_SYSTEM_MATRIX_H
#define REGULARIZED_SYSTEM_MATRIX_H

#include <math/Raytracer.h>
#include <math/Volume.h>
#include <math/AcquisitionPoses.h>
#include <math/SystemMatrix.h>

class RegularizedSystemMatrix : public SystemMatrix {
private:
	float lambda_;

public:
	RegularizedSystemMatrix(Volume_ptr volume, AcquisitionPoses acquisitionPoses, float lambda);

	virtual void apply(const Eigen::VectorXf& x, Eigen::VectorXf& b) override;
};

#endif
