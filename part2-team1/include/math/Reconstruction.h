#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <Eigen/Dense>
#include <math/Volume.h>
#include <math/IMatrix.h>
#include <math/AcquisitionPoses.h>
#include <random>
#include <math/ConjugateGradient.h>

class Reconstruction
{
private:
	Volume_ptr volume_;
	AcquisitionPoses poses_;
	int numIterations_;
	float epsilon_;
	bool useRegularizer_;
	float lambda_;
	float noise_;
	std::default_random_engine randomEngine_;

public:

	Reconstruction();

	void setVolume(const Volume_ptr& volume);
	void setAcquisitionPoses(const AcquisitionPoses& poses);
	void setNumIterations(int num_iterations);
	void setEpsilon(float epsilon);
	void setUseRegularizer(bool use_regularizer);
	void setLambda(float lambda);
	void setNoise(float noise);

	void solve(ConjugateGradient::progressListener_t progressListener);

//private:
public: //public for testing only
	Eigen::VectorXf buildRhs();
	IMatrix* buildMatrix();
};

#endif