#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <math/ConjugateGradient.h>
#include <QDebug>
#include <iostream>
#include <sstream>

void ConjugateGradient::solve(IMatrix* A, const Eigen::VectorXf& b, Eigen::VectorXf& x, 
		int numInteration, float epsilon, progressListener_t progressListener) 
{
    size_t size = b.size();
	Eigen::VectorXf d(size);
	Eigen::VectorXf Ad(size);
	Eigen::VectorXf r(size);
	Eigen::VectorXf rLast(size);
	Eigen::VectorXf Ax(size);

    double alpha, beta, rtr, rtr_1;
    //b_tmp = b;

	// r = b - B * result;  where result will be initiallized to 0

	// (45)
	A->apply(x, Ax); //to modify b
	r = b - Ax;
	d = r;

	if (!progressListener("initialized", 0)) return;

	for (int i = 0; i < numInteration; i++) {

		// (46)
		rtr = r.dot(r);
		A->apply(d,Ad);
		alpha = rtr/(d.dot(Ad));

		// (47)
		x += alpha*d;

		rtr_1 = r.dot(r); // <-- needed in (48)

		// (47)
		r -= alpha*Ad;
		
		//residual test
		float residual = r.norm();
		if (residual < epsilon) {
			break;
		}
		std::stringstream ss;
		ss << "Iteration " << (i+1) << ": residual = " << sqrt(rtr);
		std::string sss = ss.str();
		qDebug() << QString::fromStdString(sss);
		if (!progressListener(sss, (i + 1) / (float)numInteration)) break;

		// (48)
		rtr = r.dot(r);
        beta = rtr/rtr_1;

		// (49) 
		d = r + beta*d;
	}
}
