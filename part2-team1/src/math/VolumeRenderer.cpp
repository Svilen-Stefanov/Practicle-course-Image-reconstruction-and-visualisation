#include <math/VolumeRenderer.h>

#include <math/Raytracer.h> //for bounding box test
#include <math/Detector.h> //for the definition of Ray_t
#include <iostream>
#include <algorithm>

using namespace Eigen;

#define DEBUG_RENDERER 0

//--------------------------------------------------------------
//  VOLUME RENDERER IMPLEMENTATION
//--------------------------------------------------------------

VolumeRenderer_RenderSettings::VolumeRenderer_RenderSettings()
	//Set default values
	: transferFunction_(nullptr)
	, screenSize_(1,1)
	, showBoundingBox_(true)
	, stepSize_(0.1)
	, densityScale_(1)
	, blendMode_(VolumeRenderer_BlendMode::ALPHA)
	, showSlice_(false)
	, sliceHelper_(AlignedBox3f())
{
}

VolumeRenderer::VolumeRenderer()
	: managerThread_(new _VolumeRenderer_ManagerThread())
{
	connect(managerThread_, &_VolumeRenderer_ManagerThread::sendImageChanged, this, &VolumeRenderer::receiveImage);
}

VolumeRenderer::~VolumeRenderer()
{
	delete managerThread_;
}

void VolumeRenderer::paintEvent(QPainter& painter)
{
	if (settings_.volume_ == nullptr) {
		return; //not initialized yet
	}

	//fill background
	painter.fillRect(painter.window(), QColor(0, 0, 0));

	if (settings_.showBoundingBox_) 
	{
		//create bounding box lines
		const Camera& cam = settings_.camera_;
		const AlignedBox3f& box = AlignedBox3f(
			settings_.volume_->getBoundingBox().min(),
			settings_.volume_->getBoundingBox().max() - settings_.volume_->getSpacing());
		Vector3f corners[] = {
			cam.getScreenCoordinates(box.corner(AlignedBox3f::BottomLeftFloor)),
			cam.getScreenCoordinates(box.corner(AlignedBox3f::BottomLeftCeil)),
			cam.getScreenCoordinates(box.corner(AlignedBox3f::BottomRightFloor)),
			cam.getScreenCoordinates(box.corner(AlignedBox3f::BottomRightCeil)),
			cam.getScreenCoordinates(box.corner(AlignedBox3f::TopLeftFloor)),
			cam.getScreenCoordinates(box.corner(AlignedBox3f::TopLeftCeil)),
			cam.getScreenCoordinates(box.corner(AlignedBox3f::TopRightFloor)),
			cam.getScreenCoordinates(box.corner(AlignedBox3f::TopRightCeil))
		};
		struct Face_t
		{
			Vector3f a, b, c, d;
			float normalZ;
		};
		static const int faces[][4] = { { 0, 1, 2, 3 }, { 4, 6, 5, 7 }, { 0, 4, 1, 5 }, { 2, 3, 6, 7 }, { 0, 2, 4, 6 }, { 1, 5, 3, 7 } };
		std::vector<Face_t> faceVector(6);
		for (int i = 0; i < 6; ++i)
		{
			faceVector[i].a = corners[faces[i][0]];
			faceVector[i].b = corners[faces[i][1]];
			faceVector[i].c = corners[faces[i][2]];
			faceVector[i].d = corners[faces[i][3]];
			Vector3f normA = (faceVector[i].b - faceVector[i].a).cross(faceVector[i].c - faceVector[i].a);
			faceVector[i].normalZ = normA.z();
		}

		//draw faces that point away
		painter.setPen(QColor(0, 0, 150));
		for (int i = 0; i < 6; ++i) {
			if (faceVector[i].normalZ > 0)
			{
				painter.drawLine(faceVector[i].a.x(), faceVector[i].a.y(), faceVector[i].b.x(), faceVector[i].b.y());
				painter.drawLine(faceVector[i].a.x(), faceVector[i].a.y(), faceVector[i].c.x(), faceVector[i].c.y());
				painter.drawLine(faceVector[i].d.x(), faceVector[i].d.y(), faceVector[i].b.x(), faceVector[i].b.y());
				painter.drawLine(faceVector[i].d.x(), faceVector[i].d.y(), faceVector[i].c.x(), faceVector[i].c.y());
			}
		}

		//draw image
		painter.drawImage(QRectF(0, 0, img_.width(), img_.height()), img_);
		if (settings_.showSlice_) paintSliceOutline(painter);

		//draw 9 edges that are in front
		painter.setPen(QColor(255, 0, 0));
		for (int i = 0; i < 6; ++i) {
			if (faceVector[i].normalZ < 0) {
				painter.drawLine(faceVector[i].a.x(), faceVector[i].a.y(), faceVector[i].b.x(), faceVector[i].b.y());
				painter.drawLine(faceVector[i].a.x(), faceVector[i].a.y(), faceVector[i].c.x(), faceVector[i].c.y());
				painter.drawLine(faceVector[i].d.x(), faceVector[i].d.y(), faceVector[i].b.x(), faceVector[i].b.y());
				painter.drawLine(faceVector[i].d.x(), faceVector[i].d.y(), faceVector[i].c.x(), faceVector[i].c.y());
			}
		}
	} else
	{
		//just show image
		painter.drawImage(QRectF(0, 0, img_.width(), img_.height()), img_);
		if (settings_.showSlice_) paintSliceOutline(painter);
	}
}

void VolumeRenderer::paintSliceOutline(QPainter& painter)
{
	std::vector<Vector3f> points = settings_.sliceHelper_.computeIntersectionPolygon();
	painter.setPen(QColor(0, 200, 0));
	for (size_t i = 0; i < points.size(); ++i)
	{
		Vector3f a = settings_.camera_.getScreenCoordinates(points[i]);
		Vector3f b = settings_.camera_.getScreenCoordinates(points[(i + 1) % points.size()]);
		painter.drawLine(a.x(), a.y(), b.x(), b.y());
	}
}

void VolumeRenderer::notifyChange(VolumeRenderer_flag_t change)
{
	managerThread_->notifyChange(settings_, change);
	if (change & PFLAG_BOUNDING_BOX)
	{
		emit sendImageChanged(); //trigger a repaint
	}
}

void VolumeRenderer::receiveImage(const QImage& img)
{
	img_ = img;
	emit sendImageChanged();
}

//--------------------------------------------------------------
//  PRIVATE IMPLEMENTATIONS
//--------------------------------------------------------------

_VolumeRenderer_ManagerThread::_VolumeRenderer_ManagerThread()
	: abort_(false)
	, restart_(false)
	, flags_(0)
	, workPhase_(0)
	, workStep_(0)
{
}

_VolumeRenderer_ManagerThread::~_VolumeRenderer_ManagerThread()
{
	mutex_.lock();
	abort_ = true;
	condition_.wakeOne();
	mutex_.unlock();

	wait();
}

void _VolumeRenderer_ManagerThread::notifyChange(const VolumeRenderer_RenderSettings& settings, VolumeRenderer_flag_t flags)
{
	QMutexLocker locker(&mutex_);

	settings_ = settings;
	flags_ |= flags;

	if (settings_.volume_ == nullptr) {
		return; //not initialized yet
	}

	if (flags_ == VolumeRenderer::PFLAG_BOUNDING_BOX) {
		return; //only bounding box has changed, don't recompute, only redraw
	}

	if (!isRunning()) {
		restart_ = false;
		start(LowPriority);
	}
	else {
		restart_ = true;
		condition_.wakeOne();
	}
}

void _VolumeRenderer_ManagerThread::run()
{
	QImage img;

	forever{
		//get a copy of the render settings
		mutex_.lock();
		VolumeRenderer_RenderSettings settings = settings_;
		VolumeRenderer_flag_t flags = flags_;
		flags_ = 0;
		mutex_.unlock();

#if DEBUG_RENDERER
		qDebug() << "volume renderer triggered, flags:"
			<< ((flags&VolumeRenderer::PFLAG_TRACING) ? " Tracing" : "")
			<< ((flags&VolumeRenderer::PFLAG_SIZE) ? " Size" : "")
			<< ((flags&VolumeRenderer::PFLAG_RAYS) ? " Rays" : "")
			<< ((flags&VolumeRenderer::PFLAG_SLICE) ? " Slices" : "");
#endif

		//compute
		if (flags & VolumeRenderer::PFLAG_SIZE)
		{ // screen resized
			img = QImage(settings.screenSize_.x(), settings.screenSize_.y(), QImage::Format_RGBA8888);
		}

		bool refinementNeeded = false;
		//compute in parallel
		changeWorkItems(settings, flags);
		//qDebug() << "phase: " << workPhase_ << ", step: " << workStep_;
		int size = workItems_.size();
		QRgb* output = (QRgb*) img.bits();
		int scanline = img.bytesPerLine() / sizeof(QRgb);
#pragma omp parallel for schedule(dynamic)
		for (int i=0; i<size; ++i)
		{
			refineWorkItem(i, settings, output, scanline);
		}
		refinementNeeded = advancePhase();

		//qDebug() << "Done computing";

		//send result
		emit sendImageChanged(img);

		//wait for restart
		mutex_.lock();
		if (!restart_ && !abort_ && !refinementNeeded)
			condition_.wait(&mutex_);
		restart_ = false;
		bool abort = abort_;
		mutex_.unlock();
		if (abort) break;
	}
	qDebug() << "manager thread stopped";
}

void _VolumeRenderer_ManagerThread::setupWorkItems(const VolumeRenderer_RenderSettings& settings)
{
	typedef _VolumeRenderer_WorkItem WorkItem;
	workItems_.clear();
	workItems_.reserve(settings.screenSize_.x()*settings.screenSize_.y() / (2 * WORK_ITEM_SIZE));
	for (int y = 0; y < settings.screenSize_.y(); y += WORK_ITEM_SIZE) {
		for (int x = 0; x < settings.screenSize_.x(); x += WORK_ITEM_SIZE) {
			WorkItem item;
			item.start_ = Vector2i(x, y);
			item.end_ = Vector2i(std::min(settings.screenSize_.x(), x + WORK_ITEM_SIZE), std::min(settings.screenSize_.y(), y + WORK_ITEM_SIZE));
			item.rays_.resize(WORK_ITEM_SIZE * WORK_ITEM_SIZE + 4);
			workItems_.push_back(item);
		}
	}
}

void _VolumeRenderer_ManagerThread::changeWorkItems(const VolumeRenderer_RenderSettings& settings, VolumeRenderer_flag_t flags)
{
	if (flags & VolumeRenderer::PFLAG_SIZE) //recreate work items
	{
		setupWorkItems(settings);
		workPhase_ = 0;
		workStep_ = 0;
	} else if (flags & VolumeRenderer::PFLAG_RAYS) //recompute rays
	{
		workPhase_ = 0;
		workStep_ = 0;
		for (auto& wi : workItems_)
			for (auto& ray : wi.rays_)
				ray.rayComputed_ = false;
	} else if (flags & VolumeRenderer::PFLAG_TRACING || flags & VolumeRenderer::PFLAG_SLICE) //restart tracing
	{
		workPhase_ = 2;
		workStep_ = 0;
	}
}

void _VolumeRenderer_ManagerThread::refineWorkItem(int index, const VolumeRenderer_RenderSettings& settings,
	QRgb* colors, size_t scanline)
{
	typedef _VolumeRenderer_WorkItem WorkItem;
	typedef _VolumeRenderer_RayInfo RayInfo;
	WorkItem& item = workItems_[index];

	switch (workPhase_)
	{
	case 0:   //compute coarse rays
	case 2: { //compute coarse color
		Vector2i screenPositions[] = {
			item.start_ + Vector2i(WORK_ITEM_SIZE / 3, WORK_ITEM_SIZE / 3),
			item.start_ + Vector2i(WORK_ITEM_SIZE * 2 / 3, WORK_ITEM_SIZE / 3),
			item.start_ + Vector2i(WORK_ITEM_SIZE / 3, WORK_ITEM_SIZE * 2 / 3),
			item.start_ + Vector2i(WORK_ITEM_SIZE * 2 / 3, WORK_ITEM_SIZE * 2 / 3)
		};
		for (int i = 0; i < 4; ++i) {
			RayInfo& ray = item.rays_[i];
			int sx = item.start_.x() + (i % 2)*WORK_ITEM_SIZE / 2;
			int ex = std::min(settings.screenSize_.x(), sx + WORK_ITEM_SIZE / 2);
			int sy = item.start_.y() + (i / 2)*WORK_ITEM_SIZE / 2;
			int ey = std::min(settings.screenSize_.y(), sy + WORK_ITEM_SIZE / 2);
			if (!ray.rayComputed_) { //compute coarse rays
				ray.originalStart_ = settings.camera_.getWorldCoordinates(screenPositions[i].x(), screenPositions[i].y(), 0);
				ray.originalDirection_ = (settings.camera_.getWorldCoordinates(screenPositions[i].x(), screenPositions[i].y(), 1) - ray.originalStart_).normalized();
				projectRay(ray, settings.volume_);
				ray.rayComputed_ = true;
			}
			//compute coarse color
			Vector4f c;
			if (settings.showSlice_)
			{
				Eigen::Vector3f p;
				if (settings.sliceHelper_.intersectRay(ray.originalStart_, ray.originalDirection_, p))
				{
					p -= settings.volume_->getBoundingBox().min();
					p = p.array() / settings.volume_->getSpacing().array();
					float d = settings_.volume_->getVoxelInterpolated(p);
					c = settings_.transferFunction_->classify(d);
				}
				else {
					c = Vector4f(0, 0, 0, 0);
				}
			}
			else {
				c = computeRay(ray, settings);
			}
			QRgb rgb = qRgba((int)(c.x() * 255), (int)(c.y() * 255), (int)(c.z() * 255), (int)(c.w() * 255));
			for (int y = sy; y < ey; ++y)
				for (int x = sx; x < ex; ++x)
					colors[x + scanline*y] = rgb;
		}
		break;
	}
	case 1: //compute fine rays
	case 3: { //compute fine color
		int y = workStep_;
		if (y >= item.end_.y() - item.start_.y()) break;
		for (int x = 0; x < item.end_.x() - item.start_.x(); ++x) {
			int px = item.start_.x() + x;
			int py = item.start_.y() + y;
			RayInfo& ray = item.rays_[4 + x + y*WORK_ITEM_SIZE];
			if (!ray.rayComputed_)
			{
				ray.originalStart_ = settings.camera_.getWorldCoordinates(px, py, 0);
				ray.originalDirection_ = (settings.camera_.getWorldCoordinates(item.start_.x() + x, item.start_.y() + y, 1) - ray.originalStart_).normalized();
				projectRay(ray, settings.volume_);
				ray.rayComputed_ = true;
			}
			Vector4f c;
			if (settings.showSlice_) 
			{
				Eigen::Vector3f p;
				if (settings.sliceHelper_.intersectRay(ray.originalStart_, ray.originalDirection_, p)) {
					p -= settings.volume_->getBoundingBox().min();
					p = p.array() / settings.volume_->getSpacing().array();
					float d = settings_.volume_->getVoxelInterpolated(p);
					c = settings_.transferFunction_->classify(d);
				}
				else {
					c = Vector4f(0, 0, 0, 0);
				}
			}
			else
			{
				c = computeRay(ray, settings);
			}
			QRgb rgb = qRgba((int)(c.x()*255), (int)(c.y()*255), (int)(c.z()*255), (int)(c.w()*255));
			colors[px + scanline * py] = rgb;
		}
		break;
	}
	}
}

bool _VolumeRenderer_ManagerThread::advancePhase()
{
	switch (workPhase_) {
	case 0: { //compute coarse rays
		workPhase_ = 1;
		workStep_ = 0;
		return true;
	}
	case 1: { //compute fine rays
		workStep_++;
		if (workStep_ >= WORK_ITEM_SIZE)
		{
			workStep_ = 0;
			workPhase_ = 0;
			return false;
		}
		return true;
	}
	case 2: { //compute coarse color
		workPhase_ = 3;
		workStep_ = 0;
		return true;
	}
	case 3: { //compute fine color
		workStep_++;
		if (workStep_ >= WORK_ITEM_SIZE) {
			workStep_ = 4;
			workPhase_ = 0;
			return false;
		}
		return true;
	}
	}
	return false;
}

void _VolumeRenderer_ManagerThread::projectRay(_VolumeRenderer_RayInfo& ray, Volume_ptr vol) const
{
	ray.start_ = ray.originalStart_;
	ray.direction_ = ray.originalDirection_;

	ray.start_ -= vol->getBoundingBox().min();
	//ray.start_ = ray.start_.array() * factor.array();
	ray.start_ = ray.start_.array() / vol->getSpacing().array();
	ray.direction_ = ray.direction_.array() / vol->getSpacing().array();
	Vector2f collision = Raytracer::rayBoxIntersection(
		Detector::Ray_t(ray.start_, ray.direction_),
		AlignedBox3f(Vector3f(0,0,0), (vol->getNumVoxels() - Vector3i(1,1,1)).cast<float>()));
	if (collision.x() < 0)
	{
		//no collision
		ray.maxStep_ = -1;
	} 
	else
	{
		ray.start_ += ray.direction_ * collision.x(); //move start into the box
		ray.maxStep_ = collision.y() - collision.x(); //the distance to go
	}

}

Vector4f _VolumeRenderer_ManagerThread::computeRay(const _VolumeRenderer_RayInfo& ray, const VolumeRenderer_RenderSettings& settings)
{
	if (ray.maxStep_ <= 0) return Vector4f(0, 0, 0, 0);

	Vector4f color(0, 0, 0, 0);
	float stepSize = settings.stepSize_ * 0.1;
	float epsilon = NumTraits<float>::epsilon();

	switch (settings.blendMode_) //note: moving the switch outside the ray integral increases the performance
	{
	case VolumeRenderer_BlendMode::ALPHA:
		{
			color.w() = 1;
			//for (float step = 0; step < ray.maxStep_; step += settings.stepSize_*0.1) {
			for (float step = ray.maxStep_ - epsilon; step > 0; step -= settings.stepSize_*0.1) { //somehow, the rays are flipped
				Vector3f pos = ray.start_ + step*ray.direction_;
				float d = settings.volume_->getVoxelInterpolated(pos);
				Vector4f c = settings.transferFunction_->classify(d);
				c.w() *= settings.stepSize_ * settings.densityScale_;

				c.head<3>() *= c.w(); //premultiply alpha
				c.w() = 1 - c.w();

				//Front to back blending
				//Front-to-Back Blending with Early Fragment Discarding
				//Adam Vlcek, Jiri Have, Adam Herout
				color.head<3>() += color.w() * c.head<3>();
				color.w() *= c.w();
				//if (color.w() >= 1 - epsilon) break;
			}
			color.w() = 1 - color.w();
			//premultiply alpha
			color.head<3>() *= color.w();
			color.w() = 1;
		}
		break;
	case VolumeRenderer_BlendMode::ADDITIVE:
		{
			for (float step = 0; step < ray.maxStep_; step += settings.stepSize_*0.1) {
				Vector3f pos = ray.start_ + step*ray.direction_;
				float d = settings.volume_->getVoxelInterpolated(pos);
				d *= settings.stepSize_;
				d *= settings.densityScale_;
				color += settings.transferFunction_->classify(d);
			}
			color = Vector4f(1, 1, 1, 1).array().min(color.array());
		}
		break;
	case VolumeRenderer_BlendMode::ADDITIVE_DENSITY:
		{
			float density = 0;
			for (float step = 0; step < ray.maxStep_; step += settings.stepSize_*0.1) {
				Vector3f pos = ray.start_ + step*ray.direction_;
				float d = settings.volume_->getVoxelInterpolated(pos);
				density += d;
			}
			density *= settings.stepSize_;
			density *= settings.densityScale_;
			density = std::max(0.0f, std::min(1.0f, density));
			color = settings.transferFunction_->classify(density);
		}
		break;
	case VolumeRenderer_BlendMode::MAX_DENSITY:
		{
			float density = 0;
			for (float step = 0; step < ray.maxStep_; step += settings.stepSize_*0.1) {
				Vector3f pos = ray.start_ + step*ray.direction_;
				float d = settings.volume_->getVoxelInterpolated(pos);
				density = std::max(d, density);
			}
			//density *= settings.stepSize_;  //This is not needed because we use the maximum
			//density *= settings.densityScale_;
			density = std::max(0.0f, std::min(1.0f, density));
			color = settings.transferFunction_->classify(density);
		}
		break;
	}

	return color;
}
