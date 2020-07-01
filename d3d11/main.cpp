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

void Raycast(int sx, int sy, Camera* camera)
{
	//XMFLOAT4X4 P = XM
	//float vx = (+2.0f * sx / windowWidth - 1.0f) / P(0, 0); 
	//float vy = (-2.0f * sy / windowHeight + 1.0f) / P(1, 1);
}

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

	//Create test profiling
	D3D11_QUERY_DESC qd = {};
	qd.Query = D3D11_QUERY_TIMESTAMP;
	HR(dx.device->CreateQuery(&qd, &dx.startTimeQuery));
	HR(dx.device->CreateQuery(&qd, &dx.endTimeQuery));

	qd.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	HR(dx.device->CreateQuery(&qd, &dx.disjointQuery));

	D3D11_COUNTER_DESC counterDesc = {};
	counterDesc.Counter = D3D11_COUNTER_DEVICE_DEPENDENT_0;
	HR(dx.device->CreateCounter(&counterDesc, &dx.gpuCounter));

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

	ActorSystem system;
	system.CreateActors("Models/cube.obj", &dx, 5);


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

		//TODO: put into engine tick
		if (GetKeyUpState('1'))
		{
			dx.context->RSSetState(dx.rastStateWireframe);
		}
		if (GetKeyUpState('2'))
		{
			dx.context->RSSetState(dx.rastStateSolid);
		}


		//TODO: put in cammera Tick
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


		dx.Render(&camera, &ui, &system);

		win32.EndTimer();
	}

	ui.cleanup();

	return (int)msg.wParam;
}
