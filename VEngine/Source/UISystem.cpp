#include "UISystem.h"
#include "RenderSystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "Debug.h"
#include "Actor.h"
#include "WorldEditor.h"
#include "World.h"
#include "..\EditorMainWindow.h"
#include "RenderViewWidget.h"
#include "VWidget.h"

UISystem gUISystem;

void UISystem::Init()
{
	//Direct2D Init
	IDXGISurface* surface;
	
	//If D3D12 is being used, you'll get a "Not Implemented" error on GetBuffer() as the CmdAllocator is pased into the 
	//swapchain creation on d3d12 instead of the device in d3d11.
	IDXGISwapChain3* swapchain = (IDXGISwapChain3*)gRenderSystem.GetSwapchain();
	HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&surface)));

	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &d2dFactory));

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	assert(surface);
	HR(d2dFactory->CreateDxgiSurfaceRenderTarget(surface, rtProps, &d2dRenderTarget));
	surface->Release();


	//DirectWrite Init
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)(&writeFactory)));

	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 24.f, L"en-us", &textFormat));

	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-us", &textFormat));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f), &brushTransparentMenu));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &brushCloseBox));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.9f, 0.9f, 1.0f), &brushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 1.0f), &brushTextBlack));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.14f, 0.14f, 0.15f, 0.75f), &brushViewBlack));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.17f, 0.17f, 0.18f, 1.0f), &brushButton));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.1f, 0.1f, 0.8f, 1.0f), &brushCheckBoxOn));
}

void UISystem::Cleanup()
{
	d2dFactory->Release();
	d2dRenderTarget->Release();
	writeFactory->Release();

	brushTransparentMenu->Release();
	brushText->Release();
	brushTextBlack->Release();
	brushCloseBox->Release();
	brushViewBlack->Release();
	brushButton->Release();
	brushCheckBoxOn->Release();

	textFormat->Release();
}

void UISystem::Tick()
{
	GetCursorPos(&mousePos);
	ScreenToClient(gCoreSystem.mainWindow, &mousePos);

	gEditorSystem->GetMousePos((int*)&gUISystem.mousePos.x, (int*)&gUISystem.mousePos.y);
}

void UISystem::RenderText()
{

}

void UISystem::AddWidget(VWidget* widget)
{
	widgets.push_back(widget);
}

void UISystem::RenderAllWidgets(float deltaTime)
{
	for (auto& widget : widgets)
	{
		widget->Tick(deltaTime);
	}
}

void UISystem::PrintMousePos()
{
	DebugPrint("MouseX %d | MouseY %d\n", mousePos.x, mousePos.y);
}