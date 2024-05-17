#pragma once

#include <QWidget>

class RenderViewWidget : public QWidget
{
public:
	bool nativeEvent(const QByteArray&, void* message, long*) override;
	void mousePressEvent(QMouseEvent* mouseEvent) override;
	void wheelEvent(QWheelEvent* mouseWheelEvent) override;
};
