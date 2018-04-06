#ifndef SLICE_WIDGET_H
#define SLICE_WIDGET_H

#include <QtWidgets>
#include <math/Volume.h>

//This somehow combines the interface idea from
//http://stackoverflow.com/a/22618496/4053176
//with the code to paint from RealtimePreviewWidget
class AspectRatioPixmapLabel : public QWidget
{
	Q_OBJECT
public:
	explicit AspectRatioPixmapLabel(QWidget *parent = 0);
	void paintEvent(QPaintEvent *event) override;
public slots:
void setImage(const QImage &);
private:
	QImage pix;
};

class SliceWidget : public QWidget
{
	Q_OBJECT

private:
	Volume_ptr vol_;
	enum Axis
	{
		X = 0,
		Y,
		Z
	};
	Axis axis_;
	int slice_;
	QSlider* sliceSlider_;
	QComboBox* axisComboBox_;
	AspectRatioPixmapLabel* label_;
	float minValue_;
	float maxValue_;
	float scale_;

public:
	SliceWidget(QWidget* parent = nullptr);

private:
	void updateLabel();

public slots:
	void setVolume(Volume_ptr vol);

private slots:
	void setSlice(int slice);
	void setAxis(int axis);
};

#endif