
#include "RenderViewWidget.h"
#include <qevent.h>
#include "imgui_forward_declare.h"
#include "Core/Core.h"

bool RenderViewWidget::nativeEvent(const QByteArray&, void* message, long*)
{
	MSG* msg = (MSG*)message;
	if (ImGui_ImplWin32_WndProcHandler(msg->hwnd, msg->message, msg->wParam, msg->lParam))
	{
		return true;
	}

	Core::HandleWin32MessagePump(msg->message, msg->wParam, msg->lParam);

	return false;
}

void RenderViewWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() == Qt::MouseButton::LeftButton)
	{
	}
	else if (mouseEvent->button() == Qt::MouseButton::RightButton)
	{
	}
}

void RenderViewWidget::wheelEvent(QWheelEvent* mouseWheelEvent)
{
	if (mouseWheelEvent->angleDelta().y() > 0)
	{
	}
	else if (mouseWheelEvent->angleDelta().y() < 0)
	{
	}
}
