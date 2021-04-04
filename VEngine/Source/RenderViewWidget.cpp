#include "RenderViewWidget.h"
#include "imgui/backends/imgui_impl_win32.h"

RenderViewWidget::RenderViewWidget(QWidget* parent) : QWidget(parent)
{

}

void RenderViewWidget::Tick()
{
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool RenderViewWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* msg = (MSG*)message;
    if (ImGui_ImplWin32_WndProcHandler(msg->hwnd, msg->message, msg->wParam, msg->lParam))
    {
        return true;
    }

    return false;
}

void RenderViewWidget::mousePressEvent(QMouseEvent* mouseEvent)
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

void RenderViewWidget::wheelEvent(QWheelEvent* mouseWheelEvent)
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
