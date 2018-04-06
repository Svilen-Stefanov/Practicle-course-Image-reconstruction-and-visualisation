#include <math/AcquisitionPose.h>

#include <math/Raytracer.h>
#include <sstream>
#include <iomanip>

using namespace Eigen;

void AcquisitionPose::compute(Volume_ptr volume)
{
	if (output_.size() != detector_.getResolution().x()*detector_.getResolution().y())
	{
		//resize output
		output_ = VectorXf(detector_.getResolution().x()*detector_.getResolution().y());
	}

	const std::vector<Detector::Ray_t> rays = detector_.computeRays();
	const int size = (int) rays.size();
#pragma omp parallel for
	for (int i = 0; i < size; ++i)
	{
		const Raytracer raytracer(volume, rays[i]);
		output_[i] = raytracer.forwardProjection();
	}

#if 0
	//debug
	for (int y = 0; y < detector_.getResolution().y(); ++y)
	{
		std::stringstream s;
		for (int x = 0; x < detector_.getResolution().x(); ++x)
		{
			s << std::fixed << std::setw(5) << std::setprecision(2)
				<< std::setfill('0');
			s << output_[x + y*detector_.getResolution().x()] << " ";
		}
		std::cout << s.str() << std::endl;
	}
#endif
}
