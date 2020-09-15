#include "RenderViewWidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{

}

void MainWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
    {
        gInputSystem.StoreMouseLeftDownInput(VK_LBUTTON);
    }
    else if (mouseEvent->button() == Qt::MouseButton::RightButton)
    {
        gInputSystem.StoreMouseRightDownInput(VK_RBUTTON);
    }
}

void MainWidget::wheelEvent(QWheelEvent* mouseWheelEvent)
{
    if (mouseWheelEvent->angleDelta().y() > 0)
    {
        gInputSystem.StoreMouseWheelUp();
    }
    else if (mouseWheelEvent->angleDelta().y() < 0)
    {
        gInputSystem.StoreMouseWheelDown();
    }
}
