#include <math/Reconstruction.h>

#include <math/SystemMatrix.h>
#include <math/RegularizedSystemMatrix.h>
#include <math/ConjugateGradient.h>
#include <iostream>

Reconstruction::Reconstruction()
	: numIterations_(50)
	, epsilon_(0.01f)
	, lambda_(0)
	, useRegularizer_(false)
	, noise_(0)
{
}

void Reconstruction::setVolume(const Volume_ptr& volume)
{
	volume_ = volume;
}

void Reconstruction::setAcquisitionPoses(const AcquisitionPoses& poses)
{
	poses_ = poses;
}

void Reconstruction::setNumIterations(int num_iterations)
{
	numIterations_ = num_iterations;
}

void Reconstruction::setEpsilon(float epsilon)
{
	epsilon_ = epsilon;
}

void Reconstruction::setUseRegularizer(bool use_regularizer)
{
	useRegularizer_ = use_regularizer;
}

void Reconstruction::setLambda(float lambda)
{
	lambda_ = lambda;
}

void Reconstruction::setNoise(float noise)
{
	noise_ = noise;
}

void Reconstruction::solve(ConjugateGradient::progressListener_t progressListener)
{
	//rhs / b
	Eigen::VectorXf bPrime = buildRhs();
	Eigen::VectorXf b(volume_->getNumVoxels().prod());
	b.setZero();
	SystemMatrix(volume_, poses_).multTrans(bPrime, b); // x = A' * xPrime, needed for least squares approach

	//A
	IMatrix* mat = buildMatrix();

	//x
	Eigen::VectorXf x(volume_->getNumVoxels().prod());
	x.setZero();

	//solve
	ConjugateGradient::solve(mat, b, x, numIterations_, epsilon_, progressListener);

	//deconstruct
	delete mat;

	//set volume
	volume_->setContent(x);
}

Eigen::VectorXf Reconstruction::buildRhs()
{
	int s = poses_.getPoses()[0].getOutput().size();
	Eigen::VectorXf rhs(poses_.getPoses().size() * s);
	float min = std::numeric_limits<float>::max();
	float max = -min;
	for (int i = 0; i < poses_.getPoses().size(); ++i)
	{
		const Eigen::VectorXf& content = poses_.getPoses()[i].getOutput();
		for (int j = 0; j < s; ++j)
		{
			rhs[j + i*s] = content[j];
			min = std::min(min, content[j]);
			max = std::max(max, content[j]);
		}
	}
	if (noise_ > 0)
	{
		std::cout << "Add noise" << std::endl;
		//add noise
		std::normal_distribution<float> distr;
		for (int i = 0; i < rhs.size(); ++i)
		{
			rhs[i] += noise_ * (max - min) * distr(randomEngine_);
		}
	}
	return rhs;
}

IMatrix* Reconstruction::buildMatrix()
{
	if (useRegularizer_)
	{
		return new RegularizedSystemMatrix(volume_, poses_, lambda_);
	} else
	{
		return new SystemMatrix(volume_, poses_);
	}
}
