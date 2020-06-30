#include "DXUtil.h"
#include "Win32Util.h"
#include "UIContext.h"
#include "Obj.h"
#include "Camera.h"
#include "Audio.h"
#include "AudioContext.h"
#include "Input.h"
#include "WICTextureLoader.h"
#include "Actor.h"

Win32Util win32;
DXUtil dx;
UIContext ui;
AudioContext ac;



int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//WIN32 SETUP
	win32.SetupWindow(instance, cmdShow);
	win32.SetTimerFrequency();

	//D3D11 SETUP
	dx.CreateDevice();
	dx.CreateSwapchain();
	dx.CreateRTVAndDSV();
	dx.CreateShaders();
	dx.CreateInputLayout();
	dx.CreateRasterizerStates();
	dx.CreateConstantBuffer();

	//AUDIO SETUP
	ac.Init();

	Camera camera(XMVectorSet(0.f, 0.f, -5.f, 1.f));

	ui.init(dx.swapchain);

	ID3D11Resource* testTexture;
	ID3D11ShaderResourceView* testSrv;
	HR(CreateWICTextureFromFile(dx.device, L"texture.png", &testTexture, &testSrv));
	dx.context->PSSetShaderResources(0, 1, &testSrv);

	//TODO: move into dxutil
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	ID3D11SamplerState* testSampler;
	dx.device->CreateSamplerState(&sampDesc, &testSampler);
	dx.context->PSSetSamplers(0, 1, &testSampler);

	//Test actors
	Actor actor = {};
	actor.CreateActor("Models/sphere.obj", &dx);
	actor.transform.r[3] = XMVectorSet(0.f, 0.f, 15.f, 1.f);

	Actor actor2 = {};
	actor2.CreateActor("Models/cube.obj", &dx);
	actor2.transform.r[3] = XMVectorSet(5.f, 0.f, 10.f, 1.f);

		
	dx.actors.push_back(actor);
	dx.actors.push_back(actor2);

	//MAIN LOOP
	while (msg.message != WM_QUIT) 
	{
		win32.StartTimer();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//UI UPDATE
		ui.update();

		if (GetAsyncKeyState('1'))
		{
			dx.context->RSSetState(dx.rastStateWireframe);
		}
		if (GetAsyncKeyState('2'))
		{
			dx.context->RSSetState(dx.rastStateSolid);
		}


		if (GetAsyncKeyState('W'))
		{
			camera.MoveForward(5.f * win32.delta);
		}
		if (GetAsyncKeyState('S'))
		{
			camera.MoveForward(-5.f * win32.delta);
		}
		if (GetAsyncKeyState('D'))
		{
			camera.Strafe(5.f * win32.delta);
		}
		if (GetAsyncKeyState('A'))
		{
			camera.Strafe(-5.f * win32.delta);
		}
		if (GetAsyncKeyState('Q'))
		{
			camera.MoveUp(-5.f * win32.delta);
		}
		if (GetAsyncKeyState('E'))
		{
			camera.MoveUp(5.f * win32.delta);
		}

		camera.MouseMove(ui.mousePos.x, ui.mousePos.y);

		camera.UpdateViewMatrix();


		//TODO: Put render and d2d stuff into func for profiling
		ui.d2dRenderTarget->BeginDraw();
		ui.d2dRenderTarget->EndDraw();

		//RENDER
		dx.Render(&camera);

		win32.EndTimer();
	}

	ui.cleanup();

	return (int)msg.wParam;
}
