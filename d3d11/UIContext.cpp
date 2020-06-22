#include "UIContext.h"
#include "DXUtil.h"
#include "Win32Util.h"

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
	HR(writeFactory->CreateTextFormat(L"Consolas", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 18.f, L"en-us", &textFormat));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &brush));
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
	d2dRenderTarget->DrawRectangle(rect, brush);

	if (mousePos.x > rect.left && mousePos.x < rect.right)
	{
		if (mousePos.y > rect.top && mousePos.y < rect.bottom)
		{
			//if (GetKeyState(VK_LBUTTON) == 0)
			if(mouseUp)
			{
				mouseUp = false;
				return true;
			}
		}
	}

	return false;
}