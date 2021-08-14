#pragma once
#include <QWidget>

struct RenderViewWidget : public QWidget
{
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* mouseWheelEvent) override;
};
