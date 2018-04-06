#include <gui/TransferFunctionWidget.h>

#include <QDebug>
#include <iostream>

using namespace Eigen;
using namespace std;

_TransferFunctionImpl::_TransferFunctionImpl()
	: colors_(RESOLUTION, Vector4f(0,0,0,0))
{
}

Eigen::Vector4f _TransferFunctionImpl::classify(float density) const
{
	int pos = (int)(density * RESOLUTION);
	pos = max(0, min(RESOLUTION - 1, pos));
	return colors_[pos];
}

void _TransferFunctionImpl::set(int pos, int channel, float value)
{
	colors_[pos][channel] = value;
}

void _TransferFunction_Channel::reset()
{
	points_.clear();
	if (channel_ == 3) //alpha
		points_.push_back(Vector2f(0, 0));
	else
		points_.push_back(Vector2f(0, 1));
	points_.push_back(Vector2f(1, 1));
}

void _TransferFunction_Channel::draw(QPainter& p, const Vector2f& offset, const Vector2f& scale, int selectedPoint)
{
	p.setPen(color_);
	//draw lines
	for (size_t i = 1; i < points_.size(); ++i)
	{
		Vector2f a = offset.array() + points_[i - 1].array() * scale.array();
		Vector2f b = offset.array() + points_[i].array() * scale.array();
		p.drawLine(QLineF(a.x(), a.y(), b.x(), b.y()));
	}
	//draw points
	for (size_t i = 0; i < points_.size(); ++i)
	{
		Vector2f a = offset.array() + points_[i].array() * scale.array();
		qreal size = (i == selectedPoint) ? 7 : 3;
		p.drawEllipse(QPointF(a.x(), a.y()), size, size);
	}
}

void _TransferFunction_Channel::fillTF(_TransferFunctionImpl* tf)
{
	for (int i = 1; i<points_.size(); ++i) {
		Vector2f p1 = points_[i-1];
		Vector2f p2 = points_[i];
		int x1 = max(0, (int)floor(p1.x() * _TransferFunctionImpl::RESOLUTION));
		int x2 = min(_TransferFunctionImpl::RESOLUTION - 1, (int)ceil(p2.x() * _TransferFunctionImpl::RESOLUTION));
		for (int x = x1; x <= x2; ++x) {
			float v = p1.y() + (p2.y() - p1.y()) * ((x - x1) / (float)(x2 - x1));
			tf->set(x, channel_, v);
		}
	}
}

Eigen::Vector2f _TransferFunction_Channel::getPointOnScreen(int point, const Eigen::Vector2f& offset, const Eigen::Vector2f& scale)
{
	return offset.array() + points_[point].array() * scale.array();
}

TransferFunctionWidget::TransferFunctionWidget(QWidget* parent)
	: QWidget(parent)
	, selectedChannel_(3)
	, selectedPoint_(-1)
	, tf_(new _TransferFunctionImpl())
{
	setMinimumSize(200, 100);
	setBaseSize(250, 150);
	newPosition = QRectF(width() * 0.05, height() * 0.9 - 10, 10, 10);

	channels_[0].color_ = QColor(255, 0, 0);  //red
	channels_[0].channel_ = 2;
	channels_[0].reset();
	channels_[1].color_ = QColor(0, 255, 0);  //green
	channels_[1].channel_ = 1;
	channels_[1].reset();
	channels_[2].color_ = QColor(0, 0, 255);  //blue
	channels_[2].channel_ = 0;
	channels_[2].reset();
	channels_[3].color_ = QColor(128, 128, 255);  //alpha
	channels_[3].channel_ = 3;
	channels_[3].reset();

	updateTF();
}

TransferFunctionWidget::~TransferFunctionWidget()
{
	//don't delete, will crash the background thread of the renderer
	//delete tf_;
}

void TransferFunctionWidget::setupGui(QGroupBox* box)
{
	QVBoxLayout* layoutMain = new QVBoxLayout();
	layoutMain->setContentsMargins(3, 3, 3, 3);
	layoutMain->setSpacing(3);

	QHBoxLayout* controls = new QHBoxLayout();
	controls->addWidget(new QLabel("channel:"));

	selectedChannelComboBox_ = new QComboBox();
	selectedChannelComboBox_->addItem("red");
	selectedChannelComboBox_->addItem("green");
	selectedChannelComboBox_->addItem("blue");
	selectedChannelComboBox_->addItem("alpha");
	selectedChannelComboBox_->setCurrentIndex(selectedChannel_);
	controls->addWidget(selectedChannelComboBox_);
	connect(selectedChannelComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectedChannel(int)));

	controls->addStretch(1);
	resetButton_ = new QPushButton("reset");
	controls->addWidget(resetButton_);
	connect(resetButton_, SIGNAL(clicked()), this, SLOT(resetChannels()));

	layoutMain->addLayout(controls);

	layoutMain->addWidget(this, 1);

	box->setLayout(layoutMain);
}

void TransferFunctionWidget::resetChannels()
{
	for (int i = 0; i < 4; ++i)
		channels_[i].reset();
	update();
	updateTF();
}

void TransferFunctionWidget::setSelectedChannel(int channel)
{
	if (channel != selectedChannel_)
	{
		selectedChannel_ = channel;
		update();
	}
}

//http://stackoverflow.com/a/1501725
float TransferFunctionWidget::minimumDistance(const Eigen::Vector2f& a, const Eigen::Vector2f& b, const Eigen::Vector2f& p)
{
	// Return minimum distance between line segment vw and point p
	float l2 = (a - b).squaredNorm(); // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) {
		return (p - a).norm();   // v == w case
	}
	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	// We clamp t from [0,1] to handle points outside the segment vw.
	float t = max((float)0, min((float)1, (p-a).dot(b-a) / l2));
	Vector2f projection = a + ((b-a)*t);  // Projection falls on the segment
	return (p-projection).norm();
}

void TransferFunctionWidget::updateTF(int channel)
{
	if (channel == -1)
	{
		//update all
		for (int i = 0; i < 4; ++i)
			channels_[i].fillTF(tf_);
	} else
	{
		//update selected
		channels_[channel].fillTF(tf_);
	}
	emit sendTransferFunction(tf_);
}

void TransferFunctionWidget::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);
	int w = width();
	int h = height();
	QPainter painter(this);


	//transforms the space to [0,1]x[0,1]
	static const int intent = 20;
	offset_ = Vector2f(intent, h-intent);
	scale_ = Vector2f(w - 30, -(h - 30));

	//draw channels
	for (int i = 3; i >= 0; --i) {
		if (i != selectedChannel_)
		channels_[i].draw(painter, offset_, scale_, -1);
	}
	if (selectedChannel_ >= 0)
	{ //draw selected channel last
		channels_[selectedChannel_].draw(painter, offset_, scale_, selectedPoint_);
	}

	//draw key
	painter.setPen(QColor(0, 0, 0));
	painter.drawLine(QLineF(offset_.x(), offset_.y(), offset_.x()+scale_.x(), offset_.y()));
	painter.drawLine(QLineF(offset_.x(), offset_.y(), offset_.x(), offset_.y()+scale_.y()));

	QPoint zero = QPoint(10,h-10);
	painter.drawText(zero, "0");

	QPoint one = QPoint(offset_.x()+scale_.x(), h-10);
	painter.drawText(one, "1");
	one = QPoint(10, offset_.y()+scale_.y());
	painter.drawText(one, "1");

	QPoint val = QPoint(w * 0.45, h-10);
	painter.drawText(val, "d e n s i t y");
}

void TransferFunctionWidget::mousePressEvent(QMouseEvent* event)
{
	Vector2f pos = Vector2f(event->pos().x(), event->pos().y());

	static const int pointTolerance = 10;
	static const int lineTolerance = 5;

	int oldSelectedChannel = selectedChannel_;

	vector<int> channelOrder;
	if (selectedChannel_ != -1) channelOrder.push_back(selectedChannel_);
	for (int i = 0; i < 4; ++i) if (selectedChannel_ != i) channelOrder.push_back(i);

	//check if mouse is on a point
	bool pointFound = false;
	for (int c : channelOrder) {
		{
			for (int i = 0; i < channels_[c].points_.size(); ++i) {
				Vector2f p = channels_[c].getPointOnScreen(i, offset_, scale_);
				if ((p - pos).norm() < pointTolerance) {
					selectedPoint_ = i;
					selectedChannel_ = c;
					pointFound = true;
					break;
				}
			}
		}
		if (pointFound) break;
	}

	if (pointFound)
	{
		qDebug() << "Point selected: channel=" << selectedChannel_ << ", point=" << selectedPoint_;
		if (event->button() == Qt::MouseButton::RightButton)
		{
			//delete that point
			if (selectedPoint_ != 0 && selectedPoint_ != channels_[selectedChannel_].points_.size() - 1)
			{
				channels_[selectedChannel_].points_.erase(channels_[selectedChannel_].points_.begin() + selectedPoint_);
				selectedPoint_ = -1;
				updateTF(selectedChannel_);
			}
		}
	} else
	{
		bool lineFound = false;
		//check if the mouse lies on the line
		for (int c : channelOrder)
		{
			for (int i = 1; i < channels_[c].points_.size(); ++i) {
				Vector2f a = channels_[c].getPointOnScreen(i - 1, offset_, scale_);
				Vector2f b = channels_[c].getPointOnScreen(i, offset_, scale_);
				if (minimumDistance(a, b, pos) < lineTolerance)
				{
					//select that line
					selectedChannel_ = c;
					lineFound = true;
					break;
				}
			}
			if (lineFound) break;
		}
	}

	if (oldSelectedChannel != selectedChannel_)
	{
		selectedChannelComboBox_->setCurrentIndex(selectedChannel_);
	}

	update();
}

void TransferFunctionWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	int oldSelectedChannel = selectedChannel_;

	Vector2f pos = Vector2f(event->pos().x(), event->pos().y());
	static const int lineTolerance = 5;

	vector<int> channelOrder;
	if (selectedChannel_ != -1) channelOrder.push_back(selectedChannel_);
	for (int i = 0; i < 4; ++i) if (selectedChannel_ != i) channelOrder.push_back(i);

	bool lineFound = false;
	//check if the mouse lies on the line
	for (int c : channelOrder) {
		for (int i = 1; i < channels_[c].points_.size(); ++i) {
			Vector2f a = channels_[c].getPointOnScreen(i - 1, offset_, scale_);
			Vector2f b = channels_[c].getPointOnScreen(i, offset_, scale_);
			if (minimumDistance(a, b, pos) < lineTolerance) {
				//select that line
				selectedChannel_ = c;
				lineFound = true;
				//create new point
				Vector2f p = (pos.array() - offset_.array()) / scale_.array(); //transform back to [0,1]
				p.y() = min((float)1, max((float)0, p.y()));
				float xLeft = channels_[c].points_[i - 1].x();
				float xRight = channels_[c].points_[i].x();
				p.x() = max(xLeft, min(xRight, p.x()));
				channels_[c].points_.insert(channels_[c].points_.begin() + i, p);
				selectedPoint_ = i;
				updateTF(c);
				break;
			}
		}
		if (lineFound) break;
	}

	if (oldSelectedChannel != selectedChannel_) {
		selectedChannelComboBox_->setCurrentIndex(selectedChannel_);
	}

	update();
}


void TransferFunctionWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (selectedPoint_ >= 0 && selectedChannel_ >= 0)
	{
		//move that point
		Vector2f pos = Vector2f(event->pos().x(), event->pos().y());
		pos = (pos.array() - offset_.array()) / scale_.array(); //transform back to [0,1]
		pos.y() = min((float)1, max((float)0, pos.y()));
		if (selectedPoint_ == 0)
		{
			pos.x() = 0; //first point must be on the left
		} else if (selectedPoint_ == channels_[selectedChannel_].points_.size() - 1)
		{
			pos.x() = 1; //last point must be on the left
		} else
		{
			//point must lie between its left and right neightbor
			float xLeft = channels_[selectedChannel_].points_[selectedPoint_ - 1].x();
			float xRight = channels_[selectedChannel_].points_[selectedPoint_ + 1].x();
			pos.x() = max(xLeft, min(xRight, pos.x()));
		}
		//update point
		channels_[selectedChannel_].points_[selectedPoint_] = pos;
		update();
		updateTF(selectedChannel_);
	}
}

void TransferFunctionWidget::mouseReleaseEvent(QMouseEvent* event)
{
	selectedPoint_ = -1;
	update();
}