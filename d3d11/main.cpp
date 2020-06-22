#pragma comment(lib, "dwrite")

#include "DXUtil.h"
#include "Win32Util.h"
#include "UIContext.h"

UIContext ui;

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

	ui.init(dx->swapchain);

	MSG msg = {};
	while (msg.message != WM_QUIT) 
	{
		win32->StartTimer();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ui.update();
		dx->Render();

		ui.d2dRenderTarget->BeginDraw();
		if (ui.button({ 0.f, 0.f, 100.f, 100.f }))
		{
			//ui.d2dRenderTarget->DrawLine({ 0.f, 0.f }, { 600.f, 100.f }, ui.brush);
			OutputDebugString("Test");
		}
		ui.d2dRenderTarget->EndDraw();

		HR(dx->swapchain->Present(1, 0));

		win32->EndTimer();
	}

	return (int)msg.wParam;
}
