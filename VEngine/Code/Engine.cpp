#include "Engine.h"
#include "Editor/Editor.h"
#include "Core.h"
#include "Input.h"
#include "Render/Renderer.h"
#include "Asset/FBXLoader.h"
#include "UI/UISystem.h"
#include "Editor/DebugMenu.h"
#include "Camera.h"
#include "WorldEditor.h"
#include "Timer.h"
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
#include "Physics/PhysicsSystem.h"
#include "Editor/CutsceneSequencer.h"
#include "ThreadSystem.h"

Engine engine;

void Engine::Init(int argc, char* argv[])
{
	Core::Init();

	editor->Init(argc, argv);

	activeCamera = &editorCamera;

	audioSystem.Init();

	physicsSystem.Init();

	renderer.Init(editor->windowHwnd, editor->viewportWidth, editor->viewportHeight);

	fbxLoader.Init();

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

	cutsceneSequencer.PlaybackTick(deltaTime);
	activeCamera->Tick(deltaTime);

	Timer::Tick(deltaTime);

	worldEditor.Tick();
	physicsSystem.Tick(deltaTime);
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

		ThreadSystem::Tick();

		ResetSystems();

		Core::EndTimer();
	}
}

void Engine::Render(float deltaTime)
{
	renderer.Render();
	renderer.RenderParticleEmitters();

	console.InputTick();

	uiSystem.BeginDraw();
	uiSystem.DrawAllWidgets(deltaTime);
	renderer.RenderSpritesInScreenSpace();

	console.Tick();
	debugMenu.Tick(deltaTime);
	uiSystem.EndDraw();

	renderer.Present();
}

void Engine::Cleanup()
{
	physicsSystem.Cleanup();
	shaderSystem.CleanUpShaders();
	debugMenu.Cleanup();
	uiSystem.Cleanup();
}
