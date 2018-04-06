#ifndef TESTMAT_H
#define TESTMAT_H

#include <math/IMatrix.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

class TestMatrix : public IMatrix {
	private: 
		Eigen::MatrixXf acMat_;

	public:
		TestMatrix(const Eigen::Vector2i vec, const Eigen::MatrixXf mat);
		virtual ~TestMatrix() {} 

		virtual Eigen::VectorXf mult(const Eigen::VectorXf& vec);
		virtual Eigen::VectorXf multTrans(const Eigen::VectorXf& vec);

		virtual void apply(const Eigen::VectorXf& x, Eigen::VectorXf& b) override;
};

#endif
