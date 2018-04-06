#include <gui/SliceWidget.h>

#include <QDebug>

SliceWidget::SliceWidget(QWidget* parent)
	: QWidget(parent)
	, axis_(Axis::X)
	, slice_(0)
	, sliceSlider_(nullptr)
	, axisComboBox_(nullptr)
	, label_(nullptr)
{
	//create widgets
	sliceSlider_ = new QSlider(Qt::Orientation::Horizontal, this);
	sliceSlider_->setTickInterval(1);
	sliceSlider_->setMinimum(0);
	sliceSlider_->setMaximum(0);
	sliceSlider_->setValue(0);
	connect(sliceSlider_, &QSlider::valueChanged, this, &SliceWidget::setSlice);

	axisComboBox_ = new QComboBox(this);
	axisComboBox_->addItem("X");
	axisComboBox_->addItem("Y");
	axisComboBox_->addItem("Z");
	axisComboBox_->setCurrentIndex(axis_);
	connect(axisComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setAxis(int)));

	label_ = new AspectRatioPixmapLabel(this);

	//layout
	QVBoxLayout* l1 = new QVBoxLayout();
	QHBoxLayout* l2 = new QHBoxLayout();
	l2->addWidget(new QLabel("Axis:", this), 0);
	l2->addWidget(axisComboBox_, 0);
	l2->addSpacing(10);
	l2->addWidget(new QLabel("Slice:", this), 0);
	l2->addWidget(sliceSlider_, 1);
	l1->addLayout(l2, 0);
	l1->addWidget(label_, 1);
	setLayout(l1);
}

void SliceWidget::updateLabel()
{
	const Eigen::Vector3i res = vol_->getNumVoxels();

	//create image
	QImage img;
	switch (axis_)
	{
	case X:
		img = QImage(res.y(), res.z(), QImage::Format_RGB888);
		for (int y = 0; y < res.y(); ++y) {
			for (int z = 0; z < res.z(); ++z) {
				float value = vol_->getVoxel(slice_, y, z);
				value = (value - minValue_) * scale_;
				value = std::max(0.0f, std::min(1.0f, value)); //clamp to [0,1]
				int valI = (int)(value * 255);
				img.setPixel(y, z, QColor(valI, valI, valI).rgb());
			}
		}
		break;

	case Y:
		img = QImage(res.x(), res.z(), QImage::Format_RGB888);
		for (int x = 0; x < res.x(); ++x) {
			for (int z = 0; z < res.z(); ++z) {
				float value = vol_->getVoxel(x, slice_, z);
				value = (value - minValue_) * scale_;
				value = std::max(0.0f, std::min(1.0f, value)); //clamp to [0,1]
				int valI = (int)(value * 255);
				img.setPixel(x, z, QColor(valI, valI, valI).rgb());
			}
		}
		break;

	case Z:
		img = QImage(res.x(), res.y(), QImage::Format_RGB888);
		for (int x = 0; x < res.x(); ++x) {
			for (int y = 0; y < res.y(); ++y) {
				float value = vol_->getVoxel(x, y, slice_);
				value = (value - minValue_) * scale_;
				value = std::max(0.0f, std::min(1.0f, value)); //clamp to [0,1]
				int valI = (int)(value * 255);
				img.setPixel(x, y, QColor(valI, valI, valI).rgb());
			}
		}
		break;

	default:
		return;
	}

	label_->setImage(img);
	update();
}

void SliceWidget::setVolume(Volume_ptr vol)
{
	vol_ = vol;

	//find min and max
	const Eigen::Vector3i res = vol_->getNumVoxels();
	minValue_ = std::numeric_limits<float>::max();
	maxValue_ = -std::numeric_limits<float>::max();
	for (size_t i = 0; i < res.prod(); ++i) {
		float v = vol_->getContent()[i];
		minValue_ = std::min(minValue_, v);
		maxValue_ = std::max(maxValue_, v);
	}
	scale_ = 1.0 / (maxValue_ - minValue_);
	qDebug() << "volume updated, min=" << minValue_ << ", max=" << maxValue_;

	//trigger update of slider and image
	setAxis((int)axis_);
}

void SliceWidget::setSlice(int slice)
{
	slice_ = slice;
	updateLabel();
}

void SliceWidget::setAxis(int axis)
{
	axis_ = (Axis)axis;
	int maxVal = 0;
	switch (axis_)
	{
	case X: maxVal = vol_->getNumVoxels().x(); break;
	case Y: maxVal = vol_->getNumVoxels().y(); break;
	case Z: maxVal = vol_->getNumVoxels().z(); break;
	}
	if (maxVal-1 < slice_)
	{
		slice_ = maxVal - 1;
		sliceSlider_->setValue(slice_);
	}
	sliceSlider_->setMaximum(maxVal - 1);
	updateLabel();
}

AspectRatioPixmapLabel::AspectRatioPixmapLabel(QWidget *parent) :
QWidget(parent)
{
	this->setMinimumSize(1, 1);
}

void AspectRatioPixmapLabel::paintEvent(QPaintEvent* event)
{
	if (pix.isNull()) return;
	QPainter p(this);

	QRectF sourceRect(0, 0, pix.width(), pix.height());
	int w = width();
	int h = height();
	int size = std::min(w, h);
	QRectF targetRect((w - size) / 2, (h - size) / 2, size, size);

	p.drawImage(targetRect, pix, sourceRect);
}

void AspectRatioPixmapLabel::setImage(const QImage & p)
{
	pix = p;
	update();
}
