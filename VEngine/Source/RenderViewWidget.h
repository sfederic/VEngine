#pragma once

#include <qwidget.h>
#include "Input.h"
#include <QMouseEvent>

struct RenderViewWidget : public QWidget
{
    RenderViewWidget(QWidget* parent = 0);
    void Tick();
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* mouseWheelEvent) override;

    bool bRenderViewHasFocus;
};
