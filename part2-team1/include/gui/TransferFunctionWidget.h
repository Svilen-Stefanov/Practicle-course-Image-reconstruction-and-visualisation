#ifndef TRANSFER_FUNCTION_WIDGET_H
#define TRANSFER_FUNCTION_WIDGET_H

#include <QtWidgets>
#include <vector>
#include <math/ITransferFunction.h>

class TransferFunctionWidget;

class _TransferFunctionImpl : public ITransferFunction
{
public:
	static const int RESOLUTION = 256;

private:
	std::vector<Eigen::Vector4f> colors_;

public:
	_TransferFunctionImpl();
	virtual ~_TransferFunctionImpl() {}
	Eigen::Vector4f classify(float density) const override;
	void set(int pos, int channel, float value);
};

class _TransferFunction_Channel
{
private:
	friend class TransferFunctionWidget;
	QColor color_;
	int channel_;
	std::vector<Eigen::Vector2f> points_;

	void reset();
	void draw(QPainter& p, const Eigen::Vector2f& offset, const Eigen::Vector2f& scale, int selectedPoint);
	void fillTF(_TransferFunctionImpl* tf);
	Eigen::Vector2f getPointOnScreen(int point, const Eigen::Vector2f& offset, const Eigen::Vector2f& scale);
};

class TransferFunctionWidget : public QWidget
{
	Q_OBJECT;

private:
	int selectedChannel_;
	int selectedPoint_;

	QRectF newPosition;
	//Offset and scale for transforming the points to the drawing location
	Eigen::Vector2f offset_;
	Eigen::Vector2f scale_;

	_TransferFunction_Channel channels_[4];
	_TransferFunctionImpl* tf_;

	QComboBox* selectedChannelComboBox_;
	QPushButton* resetButton_;

	static float minimumDistance(const Eigen::Vector2f& a, const Eigen::Vector2f& b, const Eigen::Vector2f& p);

	void updateTF(int channel = -1);

protected:

	/*override Qt events to paint in the TransferFunction window*/
	void paintEvent(QPaintEvent* event) override;

	/*draws a point*/
	void mousePressEvent(QMouseEvent* event) override;

	/*deletes a point*/
	void mouseDoubleClickEvent(QMouseEvent* event) override;

	/*detects the change of the position of the point to be moved and paint it on it's new position*/
	void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

public:
	TransferFunctionWidget(QWidget* parent = nullptr);
	~TransferFunctionWidget();

	void setupGui(QGroupBox* box);

private slots:
	void resetChannels();
	void setSelectedChannel(int channel);

signals:
	void sendTransferFunction(const ITransferFunction* const);
};

#endif
