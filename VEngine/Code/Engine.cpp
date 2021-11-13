#include "Engine.h"
#include "Editor/Editor.h"
#include "Core.h"
#include "Input.h"
#include "Render/Renderer.h"
#include "Asset/FBXImporter.h"
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
#include "Render/SpriteSystem.h"
#include "Audio/AudioSystem.h"

Engine engine;

void Engine::Init(int argc, char* argv[])
{
	Core::Init();

	editor->Init(argc, argv);

	activeCamera = &editorCamera;

	audioSystem.Init();

	renderer.Init(editor->windowHwnd, editor->viewportWidth, editor->viewportHeight);

	fbxImporter.Init();

	uiSystem.Init((void*)renderer.swapchain);
	debugMenu.Init();

	world.Init();
	editor->UpdateWorldList();
}

void Engine::TickSystems(float deltaTime)
{
	editor->Tick();
	Core::Tick();
	commandSystem.Tick();

	audioSystem.Tick();
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
	spriteSystem.Reset();
}

void Engine::MainLoop()
{
	while (Core::mainLoop)
	{
		const float deltaTime = Core::GetDeltaTime();
		Core::StartTimer();

		TickSystems(deltaTime);
		Render(deltaTime);

		ResetSystems();

		Core::EndTimer();
	}
}

void Engine::Render(float deltaTime)
{
	renderer.Render();
	renderer.RenderParticleEmitters();

	uiSystem.BeginDraw();
	uiSystem.TickAllWidgets(deltaTime);
	renderer.RenderSpritesInScreenSpace();

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
