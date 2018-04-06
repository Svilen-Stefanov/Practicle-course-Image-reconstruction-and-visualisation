#ifndef SYSMAT_WIDGET_H
#define SYSMAT_WIDGET_H

#include <math/IMatrix.h>
#include <math/Raytracer.h>
#include <math/Volume.h>
#include <math/AcquisitionPoses.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

class SystemMatrix : public IMatrix {
private:
	Volume_ptr volume_;
	AcquisitionPoses acquisitionPoses_;
	/*
	* Stores all raytracers for all poses, in order to compute
	* forward and backward projections efficiently 
	*/
	std::vector<Raytracer> raytracers_;

public:
	SystemMatrix(Volume_ptr volume, AcquisitionPoses acquisitionPoses);
	virtual ~SystemMatrix() {}

	virtual Eigen::VectorXf mult(const Eigen::VectorXf& vec);
	virtual void multTrans(const Eigen::VectorXf& vec, Eigen::VectorXf& b);

	virtual void apply(const Eigen::VectorXf& x, Eigen::VectorXf& b) override;

};

#endif
