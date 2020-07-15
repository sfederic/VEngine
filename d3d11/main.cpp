//putting here for malloc_dbg
//#define _DEBUG 

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
#include <thread>
#include "ShaderFactory.h"
#include <omp.h>
#include "DebugMenu.h"
#include "Physics.h"

Win32Util g_win32;
DXUtil dx;
UIContext g_UIContext;
AudioContext g_AudioContext;


int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//WIN32 SETUP
	g_win32.SetupWindow(instance, cmdShow);
	g_win32.SetTimerFrequency();

	//D3D11 SETUP
	dx.CreateDevice();
	dx.CreateSwapchain();
	dx.CreateRTVAndDSV();
	dx.CreateShaders();

	dx.CreateInputLayout();
	dx.CreateRasterizerStates();

	Camera camera(XMVectorSet(0.f, 0.f, -5.f, 1.f));

	dx.CreateConstantBuffer(camera);

	//AUDIO SETUP
	g_AudioContext.Init();

	//UI SETUP
	g_UIContext.Init(dx.swapchain);

	//TEXTURE TESTING
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

	//ACTOR SYSTEM TESTING
	ActorSystem system;
	system.CreateActors("Models/ico_sphere.obj", &dx, 5);

	ID3D11Buffer* debugLinesBuffer = dx.CreateDefaultBuffer(sizeof(Vertex) * 64, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	//MAIN LOOP
	while (msg.message != WM_QUIT) 
	{
		g_win32.StartTimer();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//UI UPDATE
		g_UIContext.Update();

		if (GetMouseDownState())
		{
			for (int i = 0; i < system.actors.size(); i++)
			{
				//TODO: make this all work off of one RAYCAST call
				XMVECTOR scaleVec = XMLoadFloat3(&system.boundingBox.Extents);
				XMMATRIX boundingBoxMatrixScale = XMMatrixScalingFromVector(scaleVec);
				XMMATRIX m = system.actors[i].transform * boundingBoxMatrixScale;
				Ray ray = {};
				Raycast(ray, g_UIContext.mousePos.x, g_UIContext.mousePos.y, &camera, system.actors[i].transform);
				float dist;

				//system.boundingBox.Center = system.actors[i].GetPositionFloat3();
				//system.boundingBox.Extents = system.actors[i].GetScale();
				
				if (system.actors[i].boundingBox.Intersects(ray.origin, ray.direction, dist))
				{
					selectedActor = i;
					DrawRayDebug(ray.origin, ray.direction, dist, debugLinesBuffer);
					dx.debugLineMatrices.push_back(system.actors[i].transform);
					OutputDebugString("hit");

					break;
				}
			}
		}


		//RENDER
		dx.Tick();
		dx.Render(&camera, &g_UIContext, &system, &dx, debugLinesBuffer, g_win32.delta);


		g_win32.EndTimer();
	}

	g_UIContext.Cleanup();

	return (int)msg.wParam;
}
