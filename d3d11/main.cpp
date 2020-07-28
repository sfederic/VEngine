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

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	FBXImporter::Init();

	coreSystem.SetupWindow(instance, cmdShow);
	coreSystem.SetTimerFrequency();
	renderSystem.Init();
	audioSystem.Init();
	uiSystem.Init();

	ID3D11Buffer* debugLinesBuffer = renderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	//ACTOR SYSTEM TESTING
	ActorSystem system, system2;
	system.modelName = "cube.fbx";
	system2.modelName = "ico_sphere.fbx";
	system.CreateActors(&renderSystem, 2);
	system2.CreateActors(&renderSystem, 3);

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
		uiSystem.Tick();
		editorCamera.Tick(deltaTime);

		gWorldEditor.Tick();

		if (inputSystem.GetKeyUpState(VK_UP))
		{
			Ray ray = {};
			RaycastAll(ray, world->actorSystems[0].actors[0].GetPositionVector(),
				world->actorSystems[0].actors[0].GetForwardVector(), world);
			{
				DrawRayDebug(world->actorSystems[0].actors[0].GetPositionVector(),
					world->actorSystems[0].actors[0].GetForwardVector(), 10000.f, debugLinesBuffer);
			}
		}

		//RENDERING
		renderSystem.Tick();
		renderSystem.RenderSetup(deltaTime);
		renderSystem.RenderActorSystem(world);
		renderSystem.RenderBounds();
		renderSystem.RenderEnd(deltaTime, debugLinesBuffer);

		inputSystem.InputReset();

		coreSystem.EndTimer();
	}

	uiSystem.Cleanup();

	return (int)coreSystem.msg.wParam;
}
