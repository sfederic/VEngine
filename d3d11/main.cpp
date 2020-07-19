#include "RenderSystem.h"
#include "CoreSystem.h"
#include "UISystem.h"
#include "Obj.h"
#include "Camera.h"
#include "Audio.h"
#include "AudioSystem.h"
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

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//WIN32 SETUP
	coreSystem.SetupWindow(instance, cmdShow);
	coreSystem.SetTimerFrequency();
	
	renderSystem.Init();

	Camera camera(XMVectorSet(0.f, 0.f, -5.f, 1.f));

	//AUDIO SETUP
	audioSystem.Init();

	//UI SETUP
	uiSystem.Init(renderSystem.swapchain);

	//TEXTURE TESTING
	ID3D11Resource* testTexture;
	ID3D11ShaderResourceView* testSrv;
	HR(CreateWICTextureFromFile(renderSystem.device, L"Textures/texture.png", &testTexture, &testSrv));
	renderSystem.context->PSSetShaderResources(0, 1, &testSrv);

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	ID3D11SamplerState* testSampler;
	renderSystem.device->CreateSamplerState(&sampDesc, &testSampler);
	renderSystem.context->PSSetSamplers(0, 1, &testSampler);

	ID3D11Buffer* debugLinesBuffer = renderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	//ACTOR SYSTEM TESTING
	ActorSystem system, system2, system3;
	system3.CreateActors("Models/cylinder.obj", &renderSystem, 3);

	//World data testing
	World world = {};
	world.actorSystems.push_back(&system3);

	Ray ray = {};

	//MAIN LOOP
	while (coreSystem.msg.message != WM_QUIT)
	{
		coreSystem.StartTimer();

		coreSystem.HandleMessages();

		g_FileSystem.Tick();
		uiSystem.Tick();
		uiSystem.Tick();
		camera.Tick(&uiSystem, &coreSystem);

		//RENDER
		renderSystem.Tick();
		renderSystem.RenderSetup(&camera, &uiSystem, debugLinesBuffer, coreSystem.deltaTime);

		for (int i = 0; i < world.actorSystems.size(); i++)
		{
			renderSystem.RenderActorSystem(world.actorSystems[i], &camera);
		}

		renderSystem.RenderBounds(&world, &camera);
		renderSystem.RenderEnd(&uiSystem, &world, coreSystem.deltaTime, debugLinesBuffer, &camera);

		InputEnd();

		coreSystem.EndTimer();
	}

	uiSystem.Cleanup();

	return (int)coreSystem.msg.wParam;
}
