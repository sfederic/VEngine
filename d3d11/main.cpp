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
	cubes.modelName = "monkey.fbx";
	cubes.CreateActors(&gRenderSystem, 2);

	World* world = GetWorld();
	world->actorSystems.push_back(&cubes);
	world->actorSystems.push_back(&gWorldEditor.xAxis);
	world->actorSystems.push_back(&gWorldEditor.yAxis);
	world->actorSystems.push_back(&gWorldEditor.zAxis);

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

		//RENDERING


		gRenderSystem.Tick();
		gRenderSystem.RenderSetup(deltaTime);

		//TODO: id don't like the position of this here (here because of axis rendering)
		gWorldEditor.Tick(debugLinesBuffer);

		gRenderSystem.RenderActorSystem(world);

		gRenderSystem.RenderBounds();
		gRenderSystem.RenderEnd(deltaTime, debugLinesBuffer);

		inputSystem.InputReset();

		gCoreSystem.EndTimer();
	}

	gUISystem.Cleanup();

	return (int)gCoreSystem.msg.wParam;
}
