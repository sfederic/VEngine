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
#include "World.h"
#include "FileSystem.h"

Win32Util g_win32;
//DXUtil dx;
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
	HR(CreateWICTextureFromFile(dx.device, L"Textures/texture.png", &testTexture, &testSrv));
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

	ID3D11Buffer* debugLinesBuffer = dx.CreateDefaultBuffer(sizeof(Vertex) * 64, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	//ACTOR SYSTEM TESTING
	ActorSystem system, system2, system3;
	system.CreateActors("Models/ico_sphere.obj", &dx, 1);
	system2.CreateActors("Models/cube.obj", &dx, 2);
	system3.CreateActors("Models/cylinder.obj", &dx, 3);

	//World data testing
	World world = {};
	world.actorSystems.push_back(&system);

	//MAIN LOOP
	while (msg.message != WM_QUIT)
	{
		g_win32.StartTimer();

		//Win32 message handing
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Test actor file handling.
		if (GetKeyUpState('4'))
		{
			g_FileSystem.WriteAllActorSystems(&world, "LevelSaves/test.sav");
		}

		if (GetKeyUpState('5'))
		{
			g_FileSystem.ReadAllActorSystems(&world, "LevelSaves/test.sav");
		}

		//UI UPDATE
		g_UIContext.Update();

		camera.Tick(&g_UIContext, &g_win32);

		Ray ray = {};

		if (GetMouseUpState())
		{
			Raycast(ray, g_UIContext.mousePos.x, g_UIContext.mousePos.y, &camera, world.actorSystems[0]);
		}

		if (GetAsyncKey(VK_RIGHT))
		{
			XMVECTOR pos = world.actorSystems[0]->actors[ray.actorIndex].GetPositionVector();
			pos.m128_f32[0] += 0.15f;
			world.actorSystems[0]->actors[ray.actorIndex].SetPosition(pos);
		}
		if (GetAsyncKey(VK_LEFT))
		{
			XMVECTOR pos = world.actorSystems[0]->actors[ray.actorIndex].GetPositionVector();
			pos.m128_f32[0] -= 0.15f;
			world.actorSystems[0]->actors[ray.actorIndex].SetPosition(pos);
		}

		//RENDER
		dx.Tick();
		dx.RenderSetup(&camera, &g_UIContext, &dx, debugLinesBuffer, g_win32.delta);

		for (int i = 0; i < world.actorSystems.size(); i++)
		{
			dx.RenderActorSystem(world.actorSystems[i], &camera);
		}

		dx.RenderBounds(&world, &camera);
		dx.RenderEnd(&g_UIContext, &world, g_win32.delta);

		g_win32.EndTimer();
	}

	g_UIContext.Cleanup();

	return (int)msg.wParam;
}
