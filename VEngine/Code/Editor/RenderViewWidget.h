#pragma once

#include <QWidget>

class RenderViewWidget : public QWidget
{
public:
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* mouseWheelEvent) override;
};
