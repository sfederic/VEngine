#include "vpch.h"
#include "Engine.h"
#include <future>
#include "Core.h"
#include "Profile.h"
#include "Input.h"
#include "Camera.h"
#include "Timer.h"
#include "World.h"
#include "Log.h"
#include "Core/PropertyTypes.h"
#include "WorldEditor.h"
#include "FileSystem.h"
#include "Asset/FBXLoader.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"
#include "Render/MaterialSystem.h"
#include "Render/SpriteSystem.h"
#include "UI/UISystem.h"
#include "Editor/DebugMenu.h"
#include "Editor/Console.h"
#include "Editor/Sequencer/Sequencer.h"
#include "Editor/Editor.h"
#include "Commands/CommandSystem.h"
#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Gameplay/WorldFunctions.h"

static void ClearLog()
{
	std::ofstream ofs;
	ofs.open("Log.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

void Engine::Init(int argc, char* argv[])
{
	const auto startTime = Profile::QuickStart();

	ClearLog();
	Input::InitKeyMap();
	PropertyTypes::SetupPropertyTypesVEnum();

	WorldFunctions::SetupWorldStartFunctions();
	WorldFunctions::SetupWorldNames();

	Camera::SetEditorCameraAsActive();

	auto coreInit = std::async(std::launch::async, []() { Core::Init(); });
	auto consoleInit = std::async(std::launch::async, []() { Console::Init(); });

	coreInit.wait();
	auto audioInit = std::async(std::launch::async, []() { AudioSystem::Init(); });

	auto physicsInit = std::async(std::launch::async, []() { PhysicsSystem::Init(); });
	auto fbxInit = std::async(std::launch::async, []() { FBXLoader::Init(); });

	Editor::Get().Init(argc, argv);
	auto rendererInit = std::async(std::launch::async, []() { Renderer::Init(Editor::Get().windowHwnd, Editor::Get().GetViewportWidth(), Editor::Get().GetViewportHeight()); });

	rendererInit.wait();
	MaterialSystem::Init();

	auto debugMenuInit = std::async(std::launch::async, []() { debugMenu.Init(); });
	auto uiInit = std::async(std::launch::async, []() { UISystem::Init(Renderer::GetSwapchain()); });

	audioInit.wait();
	physicsInit.wait();
	fbxInit.wait();
	uiInit.wait();

	World::Init();

	Editor::Get().UpdateWorldList();
	Editor::Get().UpdateSystemsList();

	debugMenuInit.wait();
	consoleInit.wait();

	const double endTime = Profile::QuickEnd(startTime);
	Log("Startup took: %f seconds", endTime);
}

void Engine::TickSystems(float deltaTime)
{
	Editor::Get().Tick();
	Core::Tick();
	Input::PollInput();
	CommandSystem::Get().Tick();
	gSequencer.Tick();
	AudioSystem::Tick();

	if (!Core::gameplayOn) //Tick editor camera
	{
		Camera::GetActiveCamera().Tick(deltaTime);
	}

	Timer::Tick(deltaTime);

	WorldEditor::Tick();
	PhysicsSystem::Tick(deltaTime);
	Renderer::Tick();
	UISystem::Tick();

	if (Core::gameplayOn && !Core::IsGameWorldPaused())
	{
		World::TickAllActorSystems(deltaTime);
		World::TickAllComponentSystems(deltaTime);
	}
}

void Engine::ResetSystems()
{
	Input::Reset();
	SpriteSystem::Reset();
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

		World::DestroyAllDeferredActors();
		FileSystem::DeferredWorldLoad();

		Core::EndTimer();
	}
}

void Engine::Render(float deltaTime)
{
	Renderer::Render();

	if (!Renderer::IsRendererSetToCaptureMeshIcon())
	{
		Renderer::RenderParticleEmitters();

		Console::InputTick();

		UISystem::BeginDraw();
		UISystem::DrawAllWidgets(deltaTime);

		Renderer::RenderSpritesInScreenSpace();

		Console::Tick();
		debugMenu.Tick(deltaTime);
		UISystem::EndDraw();
	}

	Renderer::Present();
}

void Engine::Cleanup()
{
	PhysicsSystem::Cleanup();
	ShaderSystem::ClearShaders();
	debugMenu.Cleanup();
	UISystem::Cleanup();

	Renderer::Cleanup();
}
