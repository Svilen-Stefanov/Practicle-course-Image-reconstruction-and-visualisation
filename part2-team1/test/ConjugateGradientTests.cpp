#include "catch.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <math/ConjugateGradient.h>

#include "TestMatrix.h"
#include "TestUtils.h"

SCENARIO("the ConjugateGradient class solves a system correctly") {
	GIVEN("a linear system that needs to be solved") {
        Eigen::Vector2i dim;
        dim[0] = 2;
        dim[1] = 2;
        
        Eigen::Matrix2f m(2,2);
		m(0,0) = 5;
  		m(1,0) = 0;
  		m(0,1) = 0;
  		m(1,1) = 2;
  		
  		
        Eigen::VectorXf rhs(2,1);
        rhs[0] = 2; rhs[1] = 2;
        
  		Eigen::VectorXf x(2);
  		x.setZero();
 
  		Eigen::Vector2f sol;
        sol[0] = 0.08; sol[1] = 0.5;
        
        IMatrix* mat = new TestMatrix(dim,m);
		ConjugateGradient::solve(mat, rhs, x, 5, 0.01f, [](const std::string&, float){return true; });
        
        REQUIRE(sol[0] == Approx(x[0]));
        REQUIRE(sol[1] == Approx(x[1]));
	} 
	GIVEN("a second linear system that needs to be solved") {
		Eigen::Vector2i dim;
        dim[0] = 3;
        dim[1] = 3;
        //now create my matrix
        Eigen::Matrix3f m(3,3);
		m(0,0) = 2; m(0,1) = -1; m(0,2)= 0;
  		m(1,0) = -1; m(1,1) = 2; m(1,2)= -1;
  		m(2,0) = 0; m(2,1) = -1; m(2,2)= 2;
          		
  		//now the right hand side
        Eigen::VectorXf rhs(3,1);
        rhs[0] = 2;
        rhs[1] = 1;
        rhs[2] = 5;

  		Eigen::VectorXf x(3);
  		x.setZero();
 
  		Eigen::VectorXf sol(3);
        sol[0] = 5.875; sol[1] = 8.5; sol[2] = 6.625;
        
        IMatrix* mat = new TestMatrix(dim,m);

		ConjugateGradient::solve(mat, rhs, x, 10, 0.01f, [](const std::string&, float){return true; });
        
        REQUIRE(sol[0] == Approx(x[0]));
        REQUIRE(sol[1] == Approx(x[1]));
        REQUIRE(sol[2] == Approx(x[2]));
        
	}
    GIVEN("a linear system that needs to be solved") {
        Eigen::Vector2i dim;
        dim[0] = 2;
        dim[1] = 2;
        
        Eigen::Matrix2f m(2,2);
        m(0,0) = 1;
        m(1,0) = 0;
        m(0,1) = 0;
        m(1,1) = 1;
        
        
        Eigen::VectorXf rhs(2,1);
        rhs[0] = 1; rhs[1] = 1;
        
        Eigen::VectorXf x(2);
        x.setZero();
        
        Eigen::Vector2f sol;
        sol[0] = 1; sol[1] = 1;
        
        IMatrix* mat = new TestMatrix(dim,m);
		ConjugateGradient::solve(mat, rhs, x, 10, 0.01f, [](const std::string&, float){return true; });
        
        REQUIRE(sol[0] == Approx(x[0]));
        REQUIRE(sol[1] == Approx(x[1]));
    }
    
}


Eigen::MatrixXf getRandomMatrix(int n, RandomUtils& t)
{
	Eigen::MatrixXf mat(n, n);
	for (int r = 0; r < n; ++r)
	{
		float v;
		for (int c = 0; c < n; ++c)
		{
			if (r == c)
				v = 10; //diagonal
			else
				v = t.nextFloat(0, 0.2);
			mat(r, c) = v;
		}
	}
	return mat;
}
Eigen::VectorXf getRandomVector(int n, RandomUtils& t)
{
	Eigen::VectorXf v(n);
	for (int i = 0; i < n; ++i)
	{
		v(i) = t.nextFloat(-1, 1);
	}
	return v;
}
class TestMatrix2 : public IMatrix
{
private:
	Eigen::MatrixXf mat_;
public:
	TestMatrix2(const Eigen::MatrixXf& m, const Eigen::Vector2i& n) : IMatrix(n), mat_(m) {}
	virtual void apply(const Eigen::VectorXf &x, Eigen::VectorXf &b)
	{
		b = mat_ * x;
	}
};
SCENARIO("The CG solves a linear system with a random matrix")
{
	RandomUtils t;
	std::vector<float> nx{ 5, 10, 15, 20, 25, 30, 50 };
	int count = 5;
	int numIterations = 100;
	float epsilon = 1e-4;
	for (int n : nx)
	{
		for (int i = 0; i < count; ++i)
		{
			//std::cout << "n=" << n << std::endl;
			Eigen::VectorXf x(n);
			x.setZero();
			Eigen::MatrixXf mat = getRandomMatrix(n, t);
			Eigen::VectorXf b = getRandomVector(n, t);
			//std::cout << "mat: " << mat << "\nb: " << b << std::endl;
			TestMatrix2 tmat = TestMatrix2(mat, Eigen::Vector2i(n, n));
			ConjugateGradient::solve(&tmat, b, x, numIterations, epsilon, [](const std::string&, float){return true; });
			Eigen::VectorXf bPrime = mat * x;
			float residuum = (bPrime - b).norm();
			REQUIRE(residuum <= sqrt(epsilon));
		}
	}
}