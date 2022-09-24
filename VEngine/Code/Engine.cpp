#include "vpch.h"
#include "Engine.h"
#include <future>
#include "Editor/Editor.h"
#include "Core.h"
#include "Input.h"
#include "Render/Renderer.h"
#include "Render/VertexShader.h"
#include "Render/PixelShader.h"
#include "Asset/FBXLoader.h"
#include "UI/UISystem.h"
#include "Editor/DebugMenu.h"
#include "Components/CameraComponent.h"
#include "Camera.h"
#include "WorldEditor.h"
#include "Timer.h"
#include "Editor/Console.h"
#include "World.h"
#include "Render/ShaderSystem.h"
#include "Commands/CommandSystem.h"
#include "Render/SpriteSystem.h"
#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Editor/CutsceneSequencer.h"
#include "Profile.h"
#include "Gameplay/MissionSystem.h"

void Engine::Init(int argc, char* argv[])
{
	auto startTime = Profile::QuickStart();

	activeCamera = &editorCamera;

	auto coreInit = std::async(std::launch::async, []() { Core::Init(); });
	auto consoleInit = std::async(std::launch::async, []() { Console::Init(); });

	coreInit.wait();
	auto audioInit = std::async(std::launch::async, []() { audioSystem.Init(); });

	auto physicsInit = std::async(std::launch::async, []() { physicsSystem.Init(); });
	auto fbxInit = std::async(std::launch::async, []() { FBXLoader::Init(); });

	editor->Init(argc, argv);
	auto rendererInit = std::async(std::launch::async, []() { Renderer::Init(editor->windowHwnd, editor->viewportWidth, editor->viewportHeight); });

	rendererInit.wait();
	auto debugMenuInit = std::async(std::launch::async, []() { debugMenu.Init(); });
	auto uiInit = std::async(std::launch::async, []() { uiSystem.Init(Renderer::GetSwapchain()); });

	audioInit.wait();
	physicsInit.wait();
	fbxInit.wait();
	uiInit.wait();

	World::Init();
	editor->UpdateWorldList();

	debugMenuInit.wait();
	consoleInit.wait();

	MissionSystem::Init();

	double endTime = Profile::QuickEnd(startTime);
	Log("Startup took: %f seconds", endTime);
}

void Engine::TickSystems(float deltaTime)
{
	editor->Tick();
	Core::Tick();
	commandSystem.Tick();

	audioSystem.Tick();

	cutsceneSequencer.PlaybackTick(deltaTime);

	if (!Core::gameplayOn) //Tick editor camera
	{
		activeCamera->Tick(deltaTime);
	}

	Timer::Tick(deltaTime);

	WorldEditor::Tick();
	physicsSystem.Tick(deltaTime);
	Renderer::Tick();

	if (Core::gameplayOn && !Core::gameWorldPaused)
	{
		World::TickAllActorSystems(deltaTime);
		World::TickAllComponentSystems(deltaTime);
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
	Renderer::Render();
	Renderer::RenderParticleEmitters();

	Console::InputTick();

	uiSystem.BeginDraw();
	uiSystem.DrawAllWidgets(deltaTime);

	Renderer::RenderSpritesInScreenSpace();

	Console::Tick();
	debugMenu.Tick(deltaTime);
	uiSystem.EndDraw();

	Renderer::Present();
}

void Engine::Cleanup()
{
	physicsSystem.Cleanup();
	shaderSystem.ClearShaders();
	debugMenu.Cleanup();
	uiSystem.Cleanup();
}
