#pragma comment(lib, "dwrite")

#include "DXUtil.h"
#include "Win32Util.h"
#include "UIContext.h"
#include "Obj.h"

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	Win32Util* win32 = new Win32Util;
	DXUtil* dx = new DXUtil;
	UIContext* ui = new UIContext;

	win32->SetupWindow(instance, cmdShow);
	win32->SetTimerFrequency();

	dx->CreateDevice();
	dx->CreateSwapchain();
	dx->CreateRTVAndDSV();
	dx->CreateShaders();
	dx->CreateInputLayout();
	dx->CreateRasterizerState();
	//dx->CreateVertexBuffer();

	ui->init(dx->swapchain);

	OBJData model;
	if (loadOBJFile("Models/cube.obj", model))
	{
		UINT byteWidth = model.verts.size() * sizeof(float);
		dx->CreateVertexBuffer(byteWidth, model.verts.data());
	}


	while (msg.message != WM_QUIT) 
	{
		win32->StartTimer();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ui->update();
		dx->Render();

		ui->d2dRenderTarget->BeginDraw();

		ui->d2dRenderTarget->EndDraw();

		dx->context->Draw(model.verts.size() / 3, 0);

		HR(dx->swapchain->Present(1, 0));

		win32->EndTimer();
	}

	return (int)msg.wParam;
}
