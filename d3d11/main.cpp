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
#include "Console.h"

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	FBXImporter::Init();

	gCoreSystem.SetupWindow(instance, cmdShow);
	gCoreSystem.SetTimerFrequency();
	gRenderSystem.Init();
	gAudioSystem.Init();
	gUISystem.Init();
	gWorldEditor.Init();

	//TODO: This is causing a crash in Release. Is it because it's after the init code?
	ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	ActorSystem cubes;
	cubes.modelName = "cube.fbx";
	cubes.CreateActors(&gRenderSystem, 1);

	World* world = GetWorld();
	world->actorSystems.push_back(&cubes);
	//world->actorSystems.push_back(&gWorldEditor.xAxis);
	//world->actorSystems.push_back(&gWorldEditor.yAxis);
	//world->actorSystems.push_back(&gWorldEditor.zAxis);

	//MAIN LOOP
	while (gCoreSystem.msg.message != WM_QUIT)
	{
		const float deltaTime = gCoreSystem.deltaTime;

		gCoreSystem.StartTimer();
		gCoreSystem.HandleMessages();

		gFileSystem.Tick();
		gUISystem.Tick();
		editorCamera.Tick(deltaTime);
		gTimerSystem.Tick(deltaTime);

		//RENDERING
		gRenderSystem.Tick();
		gRenderSystem.RenderSetup(deltaTime);

		gWorldEditor.Tick(debugLinesBuffer);

		gRenderSystem.RenderActorSystem(world);
		gRenderSystem.RenderBounds();
		gRenderSystem.RenderEnd(deltaTime, debugLinesBuffer);

		//UI RENDERING
		gUISystem.d2dRenderTarget->BeginDraw();
		gConsole.Tick();
		gConsole.DrawViewItems();
		debugMenu.Tick(GetWorld(), deltaTime);
		gUISystem.RenderAllUIViews();
		gUISystem.d2dRenderTarget->EndDraw();

		//PRESENT
		HR(gRenderSystem.swapchain->Present(1, 0));

		gInputSystem.InputReset();

		gCoreSystem.EndTimer();
	}

	gUISystem.Cleanup();

	return (int)gCoreSystem.msg.wParam;
}
