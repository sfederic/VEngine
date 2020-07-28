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

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	FBXImporter::Init();

	coreSystem.SetupWindow(instance, cmdShow);
	coreSystem.SetTimerFrequency();
	gRenderSystem.Init();
	audioSystem.Init();
	gUISystem.Init();

	ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	//ACTOR SYSTEM TESTING
	ActorSystem system, system2;
	system.modelName = "cube.fbx";
	system2.modelName = "ico_sphere.fbx";
	system.CreateActors(&gRenderSystem, 2);
	system2.CreateActors(&gRenderSystem, 3);

	//World data testing
	World* world = GetWorld();
	//world->AddActorSystem(system);
	world->actorSystems.push_back(system);
	//world->actorSystems.push_back(system2);

	//MAIN LOOP
	while (coreSystem.msg.message != WM_QUIT)
	{
		const float deltaTime = coreSystem.deltaTime;

		coreSystem.StartTimer();
		coreSystem.HandleMessages();

		g_FileSystem.Tick();
		gUISystem.Tick();
		editorCamera.Tick(deltaTime);

		gTimerSystem.Tick(deltaTime);

		gWorldEditor.Tick();

		//RENDERING
		gRenderSystem.Tick();
		gRenderSystem.RenderSetup(deltaTime);
		gRenderSystem.RenderActorSystem(world);
		gRenderSystem.RenderBounds();
		gRenderSystem.RenderEnd(deltaTime, debugLinesBuffer);

		inputSystem.InputReset();

		coreSystem.EndTimer();
	}

	gUISystem.Cleanup();

	return (int)coreSystem.msg.wParam;
}
