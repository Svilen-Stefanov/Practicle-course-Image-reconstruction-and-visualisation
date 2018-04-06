#ifndef SIMULAT_WIDGET_H
#define SIMULAT_WIDGET_H

#include <QtGui>
#include <QtWidgets>
#include <QtConcurrent>
#include <atomic>

#include <math/Volume.h>
#include <math/AcquisitionPose.h>

//for parallel execution
class _RealtimePreviewWidget_WorkerThread : public QThread
{
	Q_OBJECT
public:
	_RealtimePreviewWidget_WorkerThread(QObject* parent);
	virtual void run() Q_DECL_OVERRIDE;
	public slots:
	void setVolume(Volume_ptr vol);
	void setDetector(const Detector& detector);
	void triggerRecompute();
signals:
	void resultReady(const QImage &i);
private:
	std::atomic_bool needsRecomputation_;
	std::atomic_bool isRunning_;
	QMutex notifyMutex_;
	QWaitCondition notifier_;
	Volume_ptr volume_;
	AcquisitionPose pose_;
};

class RealtimePreviewWidget : public QWidget {

	Q_OBJECT;

public:
	RealtimePreviewWidget(QWidget* parent = nullptr);
	~RealtimePreviewWidget();
	void paintEvent(QPaintEvent *event) override;
	static QImage computePixmap(Volume_ptr vol, AcquisitionPose& pose);

private:
	void triggerRecomputation();

	void setupParallelExecution();
	void parallelExecution();

public slots:
	void setSelectedDetector(const AcquisitionPose& pose);
	void setVolume(Volume_ptr volume);

private:
	Volume_ptr volume_;
	AcquisitionPose pose_;
	QImage pixmap_;
	float scaling_[2];

	_RealtimePreviewWidget_WorkerThread workerThread_;
};

#endif