#include "Engine.h"
#include "Editor/Editor.h"
#include "Core.h"
#include "Input.h"
#include "Render/Renderer.h"
#include "FBXImporter.h"
#include "UI/UISystem.h"
#include "Editor/DebugMenu.h"
#include "Camera.h"
#include "WorldEditor.h"
#include "TimerSystem.h"
#include "Editor/Console.h"
#include "World.h"
#include "Render/ShaderSystem.h"
#include "Components/MeshComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Commands/CommandSystem.h"
#include "Render/TextureSystem.h"
#include "Render/RenderUtils.h"
#include "Render/MaterialSystem.h"

Engine engine;

void Engine::Init(int argc, char* argv[])
{
	Core::Init();

	editor->Init(argc, argv);

	renderer.Init(editor->windowHwnd, editor->viewportWidth, editor->viewportHeight);
	RenderUtils::defaultSampler = RenderUtils::CreateSampler();

	fbxImporter.Init();

	uiSystem.Init((void*)renderer.swapchain);
	debugMenu.Init();

	activeCamera = &editorCamera;
	
	world.Init();
	editor->UpdateWorldList();
}

void Engine::TickSystems(double deltaTime)
{
	editor->Tick();
	Core::Tick();
	commandSystem.Tick();

	shaderSystem.Tick();
	activeCamera->Tick(deltaTime);
	timerSystem.Tick(deltaTime);
	worldEditor.Tick();
	renderer.Tick();

	if (Core::gameplayOn)
	{
		world.TickAllActorSystems(deltaTime);
		world.TickAllComponentSystems(deltaTime);
	}
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
	debugMenu.Tick(deltaTime);
	uiSystem.EndDraw();

	renderer.Present();
}

void Engine::Cleanup()
{
	shaderSystem.CleanUpShaders();
	debugMenu.Cleanup();
	uiSystem.Cleanup();
}
