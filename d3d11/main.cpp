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
#include "Debug.h"

//RenderSystem gRenderSystem;
//CoreSystem gCoreSystem;
//UISystem gUISystem;
//AudioSystem gAudioSystem;

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//WIN32 SETUP
	coreSystem.SetupWindow(instance, cmdShow);
	coreSystem.SetTimerFrequency();

	renderSystem.Init();

	editorCamera = Camera(XMVectorSet(0.f, 0.f, -5.f, 1.f));

	audioSystem.Init();
	uiSystem.Init();

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
	ActorSystem system;
	system.CreateActors("Models/monkey.obj", &renderSystem, 3);

	//World data testing
	World* world = GetWorld();
	world->actorSystems.push_back(&system);

	Ray ray = {};

	//MAIN LOOP
	while (coreSystem.msg.message != WM_QUIT)
	{
		const float deltaTime = coreSystem.deltaTime;

		coreSystem.StartTimer();

		coreSystem.HandleMessages();

		g_FileSystem.Tick();
		uiSystem.Tick();
		editorCamera.Tick(deltaTime);

		//RENDER
		renderSystem.Tick();
		renderSystem.RenderSetup(deltaTime);


		for (int i = 0; i < world->actorSystems.size(); i++)
		{
			renderSystem.RenderActorSystem(world->actorSystems[i]);
		}

		renderSystem.RenderBounds();
		renderSystem.RenderEnd(deltaTime);

		inputSystem.InputReset();

		coreSystem.EndTimer();
	}

	uiSystem.Cleanup();

	return (int)coreSystem.msg.wParam;
}
