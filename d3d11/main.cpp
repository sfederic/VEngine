#pragma comment(lib, "dwrite")

#include "DXUtil.h"
#include "Win32Util.h"
#include <dwrite_1.h>

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	Win32Util* win32 = new Win32Util;
	DXUtil* dx = new DXUtil;

	win32->SetupWindow(instance, cmdShow);
	win32->SetTimerFrequency();

	dx->CreateDevice();
	dx->CreateSwapchain();
	dx->CreateRTVAndDSV();
	dx->CreateShaders();
	dx->CreateInputLayout();
	dx->CreateRasterizerState();
	dx->CreateVertexBuffer();

	//Direct2D
	IDXGISurface* surface;
	HR(dx->swapchain->GetBuffer(0, IID_PPV_ARGS(&surface)));

	ID2D1Factory* d2dFactory;
	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &d2dFactory));

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	ID2D1RenderTarget* d2dRenderTarget;
	HR(d2dFactory->CreateDxgiSurfaceRenderTarget(surface, rtProps, &d2dRenderTarget));
	surface->Release();

	IDWriteFactory1* writeFactory;
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)(&writeFactory)));

	// create the DRwite text format

	IDWriteTextFormat* textFormat;
	writeFactory->CreateTextFormat(
		L"Consolas",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		18.f,
		L"en-us",
		&textFormat);

	ID2D1SolidColorBrush* brush;
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &brush));
	
	ID2D1SolidColorBrush* offBrush;
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &offBrush));

	MSG msg = {};
	while (msg.message != WM_QUIT) 
	{
		win32->StartTimer();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		dx->Render();

		d2dRenderTarget->BeginDraw();
		const WCHAR* text = L"Hello World";
		//d2dRenderTarget->DrawTextA(text, wcslen(text), textFormat, D2D1::RectF(0, 0, 800, 600), brush);
		D2D1_RECT_F rect = { 10.f, 10.f, 100.f, 100.f };
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(mainWindow, &mousePos);
		char mouse[128];
		snprintf(mouse, sizeof(mouse), "x: %d | Y: %d\n", mousePos.x, mousePos.y);
		OutputDebugString(mouse);

		d2dRenderTarget->DrawRectangle(rect, brush);


		if (mousePos.x > rect.left && mousePos.x < rect.right)
		{
			if (mousePos.y > rect.top && mousePos.y < rect.bottom)
			{
				if (msg.wParam == MK_LBUTTON)
				{
					d2dRenderTarget->FillRectangle({ 20, 20, 80, 80 }, offBrush);
				}
			}
		}

		d2dRenderTarget->EndDraw();

		HR(dx->swapchain->Present(1, 0));

		win32->EndTimer();
	}

	return (int)msg.wParam;
}
