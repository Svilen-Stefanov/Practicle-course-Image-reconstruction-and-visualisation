#ifndef ITRANSFER_FUNCTION_H
#define ITRANSFER_FUNCTION_H

#include <Eigen/Dense>

/**
 * \brief The interface for a transfer function.
 * Subclasses are passed to the Volume Renderer.
 */
class ITransferFunction
{
public:
	virtual ~ITransferFunction() {}

	/**
	 * \brief Maps the volume density to a color.
	 * This function will be called millions of times. Make it efficient!
	 * \param density the density between 0 and 1
	 * \return the color in rgba.
	 */
	virtual Eigen::Vector4f classify(float density) const = 0;
	/*
	 * Simple transfer function:
	 * get(float density) {
	 *	return Eigen::Vector4f(density, density, density, 1);
	 *	OR
	 *	return Eigen::Vector4f(1, 1, 1, density);
	 * }
	 */
};

#endif
