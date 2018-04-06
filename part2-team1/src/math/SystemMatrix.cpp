#include <math/SystemMatrix.h>

	/*used to compute the dimention*/
	static Eigen::Vector2i computeDimensionMatrix(Volume_ptr volume, AcquisitionPoses acquisitionPoses){
		return Eigen::Vector2i (volume->getNumVoxels().prod(),

		acquisitionPoses.getPoses()[0].getDetector().getResolution().prod()*
		acquisitionPoses.getPoses().size());
	}

	SystemMatrix::SystemMatrix(Volume_ptr volume, AcquisitionPoses acquisitionPoses):
	IMatrix( Eigen::Vector2i( computeDimensionMatrix(volume, acquisitionPoses) ) ),
	 volume_(volume), acquisitionPoses_(acquisitionPoses){
		 std::vector<AcquisitionPose> poses = acquisitionPoses_.getPoses();
		 //add raytracers for every pose to the raytracer vector
		 for (size_t i = 0; i < poses.size(); i++) {
			 const AcquisitionPose& pose = poses[i];
			 std::vector<Detector::Ray_t> rays = pose.getDetector().computeRays();
		 	 size_t size = rays.size();
			 for (size_t j = 0; j < size; j++) {
			 		const Raytracer raytracer(volume, rays[j]);
					raytracers_.push_back(raytracer);
			 }
		 }
	}

	Eigen::VectorXf SystemMatrix::mult(const Eigen::VectorXf& vec){
		Eigen::VectorXf result = Eigen::VectorXf(raytracers_.size());

		for (size_t i = 0; i < raytracers_.size(); i++) {
			result[i] = raytracers_[i].forwardProjection(vec);
		}

		return result;
	}

	void SystemMatrix::multTrans(const Eigen::VectorXf& vec, Eigen::VectorXf& b){
		for (size_t i = 0; i < raytracers_.size(); i++) {
			raytracers_[i].backwardProjection(vec[i], b);
		}
	}

	void SystemMatrix::apply(const Eigen::VectorXf& x, Eigen::VectorXf& b){
		b.setZero();
		multTrans(mult(x), b);
	}
