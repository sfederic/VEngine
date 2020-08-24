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
	//ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	ActorSystem cubes;
	cubes.modelName = "cube.fbx";
	cubes.CreateActors(&gRenderSystem, 2);

	World* world = GetWorld();
	world->actorSystems.push_back(&cubes);
	world->actorSystems.push_back(&gWorldEditor.xAxis);
	world->actorSystems.push_back(&gWorldEditor.yAxis);
	world->actorSystems.push_back(&gWorldEditor.zAxis);


	ID2D1BitmapRenderTarget* bitmapRt = nullptr;
	ID2D1Bitmap* bitmap = nullptr;

	HR(gUISystem.d2dRenderTarget->CreateCompatibleRenderTarget(&bitmapRt));

	HR(bitmapRt->GetBitmap(&bitmap));
	HR(bitmap->CopyFromRenderTarget(NULL, gUISystem.d2dRenderTarget, NULL));

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

		//test ortho work
		if (gInputSystem.GetKeyDownState('P'))
		{
			gRenderSystem.matrices.proj = XMMatrixOrthographicOffCenterLH(-5.f, 5.f, -5.f, 5.f, -50.f, 1000.f);
		}

		//ACTOR RENDERING
		gRenderSystem.Tick();
		gRenderSystem.RenderSetup(deltaTime);

		gWorldEditor.Tick(nullptr);

		gRenderSystem.RenderActorSystem(world);
		gRenderSystem.RenderBounds();
		gRenderSystem.RenderEnd(deltaTime, nullptr);

		//UI RENDERING
		if (gUISystem.bAllUIActive)
		{
			gUISystem.d2dRenderTarget->BeginDraw();
			gConsole.Tick();
			gConsole.DrawViewItems();
			//debugMenu.Tick(GetWorld(), deltaTime);
			gUISystem.RenderAllUIViews();
			gUISystem.d2dRenderTarget->EndDraw();
		}
		else
		{
			//CACHING TEST 
			gUISystem.d2dRenderTarget->BeginDraw();
			gUISystem.d2dRenderTarget->DrawBitmap(bitmap);
			gUISystem.d2dRenderTarget->EndDraw();
		}

		//PRESENT
		HR(gRenderSystem.swapchain->Present(1, 0));

		gInputSystem.InputReset();

		gCoreSystem.EndTimer();
	}

	gUISystem.Cleanup();

	return (int)gCoreSystem.msg.wParam;
}
