#include "UISystem.h"
#include <cassert>
#include <dxgi1_6.h>
#include "Debug.h"
#include "UI/Widget.h"
#include "Editor/Editor.h"
#include "Core.h"

UISystem uiSystem;

void UISystem::Init(void* swapchain)
{
	//Direct2D Init
	IDXGISurface* surface;

	//If D3D12 is being used, you'll get a "Not Implemented" error on GetBuffer() as the CmdAllocator is pased into the 
	//swapchain creation on d3d12 instead of the device in d3d11. The workaround is to use D3D11on12 but it uses
	//fucking 500 MB while running. I guess at that point, you'd be doing your own UI calls through D3D12 anyway.
	IDXGISwapChain3* swapchain3 = (IDXGISwapChain3*)swapchain;
	HR(swapchain3->GetBuffer(0, IID_PPV_ARGS(&surface)));

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

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.9f, 0.9f, 1.0f), &brushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.1f, 1.0f, 0.4f, 1.0f), &debugBrushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f), &brushShapes));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f), &brushShapesAlpha));
}

void UISystem::BeginDraw()
{
	d2dRenderTarget->BeginDraw();
}

void UISystem::AddWidget(Widget* widgetToAdd)
{
	for (auto widget : widgetsInViewport)
	{
		if (widget == widgetToAdd)
		{
			return;
		}
	}

	widgetsInViewport.push_back(widgetToAdd);
}

void UISystem::RemoveWidget(Widget* widgetToRemove)
{
	for (int i = 0; i < widgetsInViewport.size(); i++)
	{
		if (widgetsInViewport[i] == widgetToRemove)
		{
			widgetsInViewport.erase(widgetsInViewport.begin() + i, widgetsInViewport.end());
		}
	}
}

void UISystem::Reset()
{
	//Skip during gameplay because screen resizes cause all gameplay widgets to be deleted otherwise
	if (Core::gameplayOn) return;

	for (auto widget : widgets)
	{
		delete widget;
	}

	widgets.clear();
	widgetsInViewport.clear();
}

void UISystem::RemoveAllWidgets()
{
	widgetsInViewport.clear();
}

void UISystem::TickAllWidgets(float deltaTime)
{
	for (auto widget : widgetsInViewport)
	{
		if (widget->render)
		{
			widget->Tick(deltaTime);
		}
	}
}

void UISystem::EndDraw()
{
	d2dRenderTarget->EndDraw();
}

void UISystem::Cleanup()
{
	Reset();

	d2dFactory->Release();
	d2dRenderTarget->Release();
	writeFactory->Release();

	brushText->Release();
	debugBrushText->Release();
	brushShapes->Release();
	brushShapesAlpha->Release();

	textFormat->Release();
}
