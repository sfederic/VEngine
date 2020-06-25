#include "DXUtil.h"
#include "Win32Util.h"
#include "UIContext.h"
#include "Obj.h"
#include "Camera.h"

//Temp constant buffer
struct Matrices
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX mvp;
}matrices;

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

	Camera camera = {};
	camera.focusPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	camera.location = XMVectorSet(0.f, 0.f, -5.f, 1.f);
	camera.worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	matrices.model = XMMatrixIdentity();
	camera.view = XMMatrixLookAtLH(camera.location, camera.focusPoint, camera.worldUp);
	matrices.view = camera.view;
	matrices.proj = XMMatrixPerspectiveFovLH(XM_PI / 3, win32->GetAspectRatio(), 0.01f, 1000.f);
	matrices.mvp = matrices.model * matrices.view * matrices.proj;

	ID3D11Buffer* cbMatrices = dx->CreateDefaultBuffer(sizeof(Matrices), D3D11_BIND_CONSTANT_BUFFER, &matrices);
	dx->context->VSSetConstantBuffers(0, 1, &cbMatrices);
	
	ui->init(dx->swapchain);

	//Temp model loading
	OBJData model;
	if (loadOBJFile("Models/sphere.obj", model))
	{
		UINT byteWidth = model.GetByteWidth();
		dx->CreateVertexBuffer(byteWidth, model.verts.data());
	}

	//MAIN LOOP
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

		if (GetAsyncKeyState(VK_UP))
		{
			//camera.
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			camera.location.m128_f32[0] += 0.05f;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			camera.location.m128_f32[0] -= 0.05f;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			camera.location.m128_f32[1] -= 0.05f;
		}

		//camera.MouseMove()

		camera.UpdateViewMatrix();
		//matrices.view = XMMatrixLookAtLH(camera.location, camera.focusPoint, camera.worldUp);
		matrices.view = camera.view;
		matrices.mvp = matrices.model * matrices.view * matrices.proj;
		dx->context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);

		ui->d2dRenderTarget->BeginDraw();

		ui->d2dRenderTarget->EndDraw();

		dx->context->Draw(model.verts.size(), 0);

		HR(dx->swapchain->Present(1, 0));

		win32->EndTimer();
	}

	ui->cleanup();

	return (int)msg.wParam;
}
