#include <QtWidgets>
#include <iostream>
#include <algorithm>

#include <gui/RealtimePreviewWidget.h>

#ifndef PREVIEW_PARALLEL
// 1: computation in parallel
// 0: computation seriell
#define PREVIEW_PARALLEL 1
#endif

RealtimePreviewWidget::RealtimePreviewWidget(QWidget *parent)
	: QWidget(parent)
	, workerThread_(this)
{
	setupParallelExecution();
}

RealtimePreviewWidget::~RealtimePreviewWidget() {

}

void RealtimePreviewWidget::paintEvent(QPaintEvent* event)
{
	if (pixmap_.isNull()) return;
	QPainter p(this);

	QRectF sourceRect(0, 0, pixmap_.width(), pixmap_.height());
	int w = width();
	int h = height();
	int size = std::min(w, h);
	QRectF targetRect((w - size) / 2, (h - size) / 2, size, size);
	
	p.drawImage(targetRect, pixmap_, sourceRect);
}

void RealtimePreviewWidget::triggerRecomputation()
{
	if (volume_ == nullptr) return;
#if PREVIEW_PARALLEL==0
	pixmap_ = computePixmap(volume_, pose_);
	repaint();
#else
	workerThread_.setDetector(pose_.getDetector());
	workerThread_.triggerRecompute();
#endif
}

QImage RealtimePreviewWidget::computePixmap(Volume_ptr vol, AcquisitionPose& pose)
{
	const Eigen::Vector2i resolution = pose.getDetector().getResolution();
	QImage p(resolution.x(), resolution.y(), QImage::Format_RGB888);
	pose.compute(vol);
	Eigen::VectorXf output = pose.getOutput();
	//first pass, compute max value
	float maxVal = std::numeric_limits<float>::epsilon();
	for (int i = 0; i < output.size(); ++i)
		maxVal = std::max(maxVal, output[i]);

	//second pass, write pixels
	for (int y = 0; y < resolution.y(); ++y)
	{
		for (int x = 0; x < resolution.x(); ++x)
		{
			float value = output[x + resolution.x()*y];
			value /= maxVal;
			int valI = (int) (std::max((float)0, std::min((float)1, value)) * 255);
			p.setPixel(x, y, QColor(valI, valI, valI).rgb());
		}
	}
	return p;
}

void RealtimePreviewWidget::setupParallelExecution()
{
	//start thread
#if PREVIEW_PARALLEL==1
	connect(&workerThread_, &_RealtimePreviewWidget_WorkerThread::resultReady, this, [this](const QImage &i)
	{
		pixmap_ = i;
		repaint();
	});
	connect(&workerThread_, &_RealtimePreviewWidget_WorkerThread::finished, &workerThread_, &QObject::deleteLater);
	workerThread_.start();
#endif
}

void RealtimePreviewWidget::parallelExecution()
{
}

void RealtimePreviewWidget::setSelectedDetector(const AcquisitionPose& pose)
{
	qDebug() << "selected detector for preview changed";
	pose_ = pose;
	triggerRecomputation();
}

void RealtimePreviewWidget::setVolume(Volume_ptr volume)
{
	volume_ = volume;
	workerThread_.setVolume(volume);

#if 0 //not needed anymore, scaling is done in the image
	Eigen::Vector3i res = volume_->getNumVoxels();
	//compute scaling by size
	float factor1 = (volume_->getNumVoxels().x() + volume_->getNumVoxels().y() + volume_->getNumVoxels().z()); //scale color
	//find min and max values of the volume
	float min = std::numeric_limits<float>::max();
	float max = -std::numeric_limits<float>::min();
	for (int z = 0; z < res.z(); ++z) {
		for (int y = 0; y < res.y(); ++y) {
			for (int x = 0; x < res.x(); ++x) {
				float value = volume_->getVoxel(x, y, z);
				min = std::min(min, value);
				max = std::max(max, value);
			}
		}
	}
	//the final scaling:
	scaling_[0] = min;
	scaling_[1] = 1.0 / (factor1 * (max - min));
#endif
}

_RealtimePreviewWidget_WorkerThread::_RealtimePreviewWidget_WorkerThread(QObject* parent)
	: QThread(parent)
{
	std::atomic_init(&needsRecomputation_, false);
	std::atomic_init(&isRunning_, false);
}

void _RealtimePreviewWidget_WorkerThread::run()
{
	bool b;
	while (true)
	{
		b = true;
		notifyMutex_.lock();
		if (needsRecomputation_.compare_exchange_strong(b, false))
		{
			//compute
			isRunning_.store(true);
			notifyMutex_.unlock();
			QImage p = RealtimePreviewWidget::computePixmap(volume_, pose_);
			emit resultReady(p);
			
		} else
		{
			//wait
			isRunning_.store(false);
			notifier_.wait(&notifyMutex_);
			notifyMutex_.unlock();
		}
	}
}

void _RealtimePreviewWidget_WorkerThread::setVolume(Volume_ptr vol)
{
	volume_ = vol;
}

void _RealtimePreviewWidget_WorkerThread::setDetector(const Detector& detector)
{
	pose_.setDetector(detector);
}

void _RealtimePreviewWidget_WorkerThread::triggerRecompute()
{
	bool b = false;
	notifyMutex_.lock();
	needsRecomputation_.store(true);
	if (isRunning_.compare_exchange_strong(b, true))
	{
		notifyMutex_.unlock();
		notifier_.wakeOne();
	} else
	{
		notifyMutex_.unlock();
	}
}
