#pragma once

#include <qwidget.h>
#include "Input.h"
#include <QMouseEvent>

struct RenderViewWidget : public QWidget
{
    RenderViewWidget(QWidget* parent = 0);
    void Tick();
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void wheelEvent(QWheelEvent* mouseWheelEvent) override;
};
