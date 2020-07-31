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
	gWorldEditor.Init();

	ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	ActorSystem cubes;
	cubes.modelName = "x_axis.fbx";
	cubes.CreateActors(&gRenderSystem, 1);

	World* world = GetWorld();
	world->actorSystems.push_back(cubes);

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


		Actor* actor = world->GetActor(gWorldEditor.actorSystemIndex, gWorldEditor.actorIndex);
		if (actor)
		{
			gWorldEditor.MoveActor(actor);
		}

		//RENDERING
		gRenderSystem.Tick();
		gRenderSystem.RenderSetup(deltaTime);

		//TODO: id don't like the position of this here
		gWorldEditor.Tick();

		gRenderSystem.RenderActorSystem(world);
		gRenderSystem.RenderBounds();
		gRenderSystem.RenderEnd(deltaTime, debugLinesBuffer);

		inputSystem.InputReset();

		gCoreSystem.EndTimer();
	}

	gUISystem.Cleanup();

	return (int)gCoreSystem.msg.wParam;
}
