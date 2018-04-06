#include "TestMatrix.h"

TestMatrix::TestMatrix(const Eigen::Vector2i vec, const Eigen::MatrixXf mat) : IMatrix (vec) {
    acMat_ = mat;
}

//multiplies acMat_ by vec
Eigen::VectorXf TestMatrix::mult(const Eigen::VectorXf &vec) {
	Eigen::VectorXf u = acMat_ * vec;
    return u;
}

//multiplies acMat_ transpose by vec
Eigen::VectorXf TestMatrix::multTrans(const Eigen::VectorXf& vec) {
    Eigen::VectorXf u = acMat_.transpose() * vec;
    return u;
}

void TestMatrix::apply(const Eigen::VectorXf &x, Eigen::VectorXf &b) {
    b = multTrans(mult(x));
}