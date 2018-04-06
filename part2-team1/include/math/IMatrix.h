#ifndef IMAT_WIDGET_H
#define IMAT_WIDGET_H

#include <Eigen/Dense>
#include <Eigen/Geometry>

class IMatrix
{
private:
	Eigen::Vector2i dimension_;

protected:
	IMatrix(const Eigen::Vector2i& vec): dimension_(vec)
	{
	}

public:

	/**
	 * \brief Computes matrix times vector.
	 * \param x the input
	 * \param b the output of this*x
	 */
	virtual void apply(const Eigen::VectorXf& x, Eigen::VectorXf& b) = 0;

	Eigen::Vector2i getDimension() const { return dimension_; }

	virtual ~IMatrix()
	{
	}
};

#endif
