#include "RenderSystem.h"
#include "CoreSystem.h"
#include "UISystem.h"
#include "Obj.h"
#include "Camera.h"
#include "Audio.h"
#include "AudioSystem.h"
#include "Input.h"
#include "Actor.h"
#include <thread>
#include "ShaderFactory.h"
#include <omp.h>
#include "DebugMenu.h"
#include "Raycast.h"
#include "World.h"
#include "FileSystem.h"
#include "Debug.h"
#include "FBXImporter.h"
#include "WorldEditor.h"
#include "TimerSystem.h"
#include "MathHelpers.h"

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	FBXImporter::Init();

	gCoreSystem.SetupWindow(instance, cmdShow);
	gCoreSystem.SetTimerFrequency();
	gRenderSystem.Init();
	gAudioSystem.Init();
	gUISystem.Init();

	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	gRenderSystem.device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature));

	//ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	//ACTOR SYSTEM TESTING
	ActorSystem system;
	system.modelName = "cube.fbx";

	system.CreateActors(&gRenderSystem, 1);


	//World data testing
	World* world = GetWorld();
	world->AddActorSystem(system);


	//MAIN LOOP
	while (gCoreSystem.msg.message != WM_QUIT)
	{
		const float deltaTime = gCoreSystem.deltaTime;

		gCoreSystem.StartTimer();
		gCoreSystem.HandleMessages();

		g_FileSystem.Tick();
		gUISystem.Tick();
		editorCamera.Tick(deltaTime);

		gTimerSystem.Tick(deltaTime);

		gWorldEditor.Tick();

		Actor* actor = world->GetActor(gWorldEditor.actorSystemIndex, gWorldEditor.actorIndex);
		if (actor)
		{
			gWorldEditor.MoveActor(actor);
		}

		//RENDERING
		gRenderSystem.Tick();
		gRenderSystem.RenderSetup(deltaTime);
		gRenderSystem.RenderActorSystem(world);
		gRenderSystem.RenderBounds();
		gRenderSystem.RenderEnd(deltaTime, nullptr);

		inputSystem.InputReset();

		gCoreSystem.EndTimer();
	}

	gUISystem.Cleanup();

	return (int)gCoreSystem.msg.wParam;
}
