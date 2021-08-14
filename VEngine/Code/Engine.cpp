#include "Engine.h"
#include "Editor/Editor.h"
#include "Core.h"
#include "Input.h"
#include "Render/Renderer.h"
#include "FBXImporter.h"
#include "UISystem.h"
#include "Editor/DebugMenu.h"
#include "Camera.h"
#include "WorldEditor.h"
#include "TimerSystem.h"
#include "Editor/Console.h"
#include "World.h"

Engine engine;

void Engine::Init(int argc, char* argv[])
{
	Core::Init();

	editor->Init(argc, argv);
	renderer.Init(editor->windowHwnd, editor->viewportWidth, editor->viewportHeight);
	fbxImporter.Init();
	uiSystem.Init((void*)renderer.swapchain.Get());
	debugMenu.Init();

	activeCamera = &editorCamera;
	
	world.Start();
}

void Engine::TickSystems(double deltaTime)
{
	editor->Tick();
	activeCamera->Tick(deltaTime, editor->viewportMouseX, editor->viewportMouseY);
	timerSystem.Tick(deltaTime);
	worldEditor.Tick();
}

void Engine::ResetSystems()
{
	Input::Reset();
}

void Engine::MainLoop()
{
	while (Core::mainLoop)
	{
		const double deltaTime = Core::GetDeltaTime();
		Core::StartTimer();

		TickSystems(deltaTime);
		Render(deltaTime);

		ResetSystems();

		Core::EndTimer();
	}
}

void Engine::Render(double deltaTime)
{
	renderer.RenderSetup();
	renderer.Render();

	uiSystem.BeginDraw();
	console.Tick();
	uiSystem.EndDraw();

	debugMenu.Tick(deltaTime);

	renderer.Present();
}

void Engine::Cleanup()
{
	debugMenu.Cleanup();
	uiSystem.Cleanup();
}
