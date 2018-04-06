#ifndef CONJUGATEG_H
#define CONJUGATEG_H

#include <math/IMatrix.h>
#include <functional>
#include <string>

class ConjugateGradient
{
public:
	/**
	 * \brief Defines a progress listener callback function.
	 * It is called on every step and provides a current message and the progress in percent.
	 * It returns false if the progress is cancelled
	 */
	typedef std::function<bool(const std::string&, float)> progressListener_t;

public:
	static void solve(IMatrix* A, const Eigen::VectorXf& b, Eigen::VectorXf& x, 
		int numInteration, float epislon, progressListener_t progressListener);

};

#endif 