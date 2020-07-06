#include "UIContext.h"
#include "DXUtil.h"
#include "Win32Util.h"
#include "Input.h"

void UIContext::init(IDXGISwapChain* swapchain)
{
	//Direct2D Init
	IDXGISurface* surface;
	HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&surface)));

	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &d2dFactory));

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	HR(d2dFactory->CreateDxgiSurfaceRenderTarget(surface, rtProps, &d2dRenderTarget));
	surface->Release();

	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)(&writeFactory)));

	//DirectWrite Init
	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-us", &textFormat));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f), &brushTransparentMenu));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.9f, 0.9f, 1.0f), &brushText));
}

void UIContext::cleanup()
{
	d2dFactory->Release();
	d2dRenderTarget->Release();
	writeFactory->Release();

	brushTransparentMenu->Release();
	brushText->Release();
	textFormat->Release();
}

void UIContext::update()
{
	GetCursorPos(&mousePos);
	ScreenToClient(mainWindow, &mousePos);
}

void UIContext::renderStart()
{
	d2dRenderTarget->BeginDraw();
}

void UIContext::renderEnd()
{
	d2dRenderTarget->EndDraw();
}

bool UIContext::button(D2D1_RECT_F rect)
{
	if (mousePos.x > rect.left && mousePos.x < rect.right)
	{
		if (mousePos.y > rect.top && mousePos.y < rect.bottom)
		{
			d2dRenderTarget->DrawRectangle(rect, brushTransparentMenu);

			if(GetMouseUpState())
			{ 
				return true;
			}

			return false;
		}
	}

	d2dRenderTarget->DrawRectangle(rect, brushText);
	return false;
	
}