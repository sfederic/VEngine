#include "RenderViewWidget.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "../EditorMainWindow.h"
#include <QSpinBox>
#include <QVBoxLayout>
#include "RenderSystem.h"
#include "CoreSystem.h"
#include "UISystem.h"
#include "Camera.h"
#include "EditorSystem.h"

RenderViewWidget::RenderViewWidget(QWidget* parent) : QWidget(parent)
{
    //TODO: Need some snap setting controls. See what the tradeoffs are between Qt and IMGUI
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

    switch (msg->message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);

    case WM_KEYDOWN:
        gInputSystem.StoreKeyDownInput(msg->wParam);

        //Close editor
        if (msg->wParam == VK_ESCAPE)
        {

        }

        break;

    case WM_KEYUP:
        gInputSystem.StoreKeyUpInput(msg->wParam);
        break;

    //TODO: there's a bit of conflict here between 
    case WM_LBUTTONUP:
        //gInputSystem.StoreMouseLeftUpInput(msg->wParam);
        //break;

    case WM_LBUTTONDOWN:
        //gInputSystem.StoreMouseLeftDownInput(msg->wParam);
        //break;

    case WM_RBUTTONUP:
        gInputSystem.StoreMouseRightUpInput();
        break;

    case WM_RBUTTONDOWN:
        gInputSystem.StoreMouseRightDownInput();
        break;

    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(msg->wParam) < 0)
        {
            gInputSystem.StoreMouseWheelDown();
        }
        else
        {
            gInputSystem.StoreMouseWheelUp();
        }
        break;

    case WM_SIZE:
        if (gRenderSystem.swapchain)
        {
            gRenderSystem.context->OMSetRenderTargets(0, 0, 0);

            // Release all outstanding references to the swap chain's buffers.
            for (int rtvIndex = 0; rtvIndex < gRenderSystem.frameCount; rtvIndex++)
            {
                gRenderSystem.rtvs[rtvIndex]->Release();
            }

            gRenderSystem.dsv->Release();

            //gUISystem.Cleanup();
            //gUISystem.Init();
            
            //ID3D11Debug* debug;
            //gRenderSystem.device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
            //debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
            //gRenderSystem.context->ClearState();
            //gRenderSystem.context->Flush();
            
            // Preserve the existing buffer count and format.
            // Automatically choose the width and height to match the client rect for HWNDs.
            gEditorSystem->SetWindowWidthHeight();

            HR(gRenderSystem.swapchain->ResizeBuffers(gRenderSystem.frameCount,
                gCoreSystem.windowWidth, gCoreSystem.windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

            gRenderSystem.CreateRTVAndDSV();

            gRenderSystem.matrices.proj = XMMatrixPerspectiveFovLH(XM_PI / 3, gCoreSystem.GetAspectRatio(), 0.01f, 1000.f);
            GetActiveCamera()->proj = gRenderSystem.matrices.proj;

            // Set up the viewport.
            gRenderSystem.viewport.Width = size().width();
            gRenderSystem.viewport.Height = size().height();
            gRenderSystem.viewport.MinDepth = 0.0f;
            gRenderSystem.viewport.MaxDepth = 1.0f;
            gRenderSystem.viewport.TopLeftX = 0;
            gRenderSystem.viewport.TopLeftY = 0;
            gRenderSystem.context->RSSetViewports(1, &gRenderSystem.viewport);
        }

        break;
    }

    return false;
}

void RenderViewWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
    {
        gInputSystem.StoreMouseLeftDownInput();
    }
    else if (mouseEvent->button() == Qt::MouseButton::RightButton)
    {
        gInputSystem.StoreMouseRightDownInput();
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
