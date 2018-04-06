#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <Eigen/Dense>
#include <Eigen/Geometry>

class RandomUtils
{
private:
	std::default_random_engine engine_;

	std::uniform_int_distribution<int> intDistr_;
	std::uniform_real_distribution<float> floatDistr_;
	std::uniform_real_distribution<double> doubleDistr_;

public:
	int nextInt(int min = 0, int max = std::numeric_limits<int>::max())
	{
		int val = intDistr_(engine_);
		return (val % (max - min)) + min;
	}
	float nextFloat(float min = 0, float max = 1)
	{
		float val = floatDistr_(engine_);
		return std::fmod(val, max - min) + min;
	}
	double nextDouble(double min = 0, double max = 1)
	{
		double val = doubleDistr_(engine_);
		return std::fmod(val, max - min) + min;
	}

	template<typename T>
	T next(T min, T max);

	//https://github.com/erich666/GraphicsGems/blob/master/gemsiii/urot.c
	template<typename Real>
	Eigen::Quaternion<Real> nextRandomRotation()
	{
		static const Real TWO_PI = 3.14159265358979323846 * 2;
		Real a = next<Real>(0, 1);
		Real b = next<Real>(0, 1);
		Real c = next<Real>(0, 1);
		Real r1 = std::sqrt(1.f - a), r2 = std::sqrt(a);
		Real t1 = TWO_PI*b, t2 = TWO_PI*c;
		Real c1 = std::cos(t1), s1 = std::sin(t1);
		Real c2 = std::cos(t2), s2 = std::sin(t2);
		return Eigen::Quaternion<Real>(c2*r2, s1*r1, c1*r1, s2*r2);
	}
};

template<>
inline int RandomUtils::next(int min, int max) { return nextInt(min, max); }
template<>
inline float RandomUtils::next(float min, float max) { return nextFloat(min, max); }
template<>
inline double RandomUtils::next(double min, double max) { return nextDouble(min, max); }

#endif