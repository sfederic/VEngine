#include <fstream>
#include <future>
import Core.Engine;

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
	Input::Init();
	PropertyTypes::SetupPropertyTypesVEnum();

	WorldFunctions::SetupWorldStartFunctions();
	WorldFunctions::SetupWorldNames();

	Camera::SetEditorCameraAsActive();

	//Keep core and audio inits here non-async. The reason is that XAudio is waiting on CoInitializeEx(),
	//and the threading makes it act funny when COINIT_APARTMENTTHREADED is an arg, which is used because of Qt
	//working with Windows native file dialogs.
	Core::Init();
	AudioSystem::Init();

	auto consoleInit = std::async(std::launch::async, []() { Console::Init(); });

	auto physicsInit = std::async(std::launch::async, []() { PhysicsSystem::Init(); });
	auto fbxInit = std::async(std::launch::async, []() { FBXLoader::Get().Init(); });

	Editor::Get().Init(argc, argv);
	auto rendererInit = std::async(std::launch::async, []() { Renderer::Get().Init(Editor::Get().windowHwnd, Editor::Get().GetViewportWidth(), Editor::Get().GetViewportHeight()); });

	rendererInit.wait();
	MaterialSystem::Init();

	auto debugMenuInit = std::async(std::launch::async, []() { debugMenu.Init(); });
	auto uiInit = std::async(std::launch::async, []() { UISystem::Init(Renderer::Get().GetSwapchain()); });

	physicsInit.wait();
	fbxInit.wait();
	uiInit.wait();

	World::Get().Init();

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
	Renderer::Get().Tick();
	UISystem::Tick();

	if (Core::gameplayOn && !Core::IsGameWorldPaused())
	{
		World::Get().TickAllActorSystems(deltaTime);
		World::Get().TickAllComponentSystems(deltaTime);
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

		World::Get().DestroyAllDeferredActors();
		FileSystem::DeferredWorldLoad();

		Core::EndTimer();
	}
}

void Engine::Render(float deltaTime)
{
	Renderer::Get().Render();

	if (!Renderer::Get().IsRendererSetToCaptureMeshIcon())
	{
		Renderer::Get().RenderParticleEmitters();

		Console::InputTick();

		UISystem::BeginDraw();
		UISystem::DrawAllWidgets(deltaTime);

		Renderer::Get().RenderSpritesInScreenSpace();

		Console::Tick();
		debugMenu.Tick(deltaTime);
		UISystem::EndDraw();
	}

	Renderer::Get().Present();
}

void Engine::Cleanup()
{
	PhysicsSystem::Cleanup();
	ShaderSystem::ClearShaders();
	debugMenu.Cleanup();
	UISystem::Cleanup();

	Renderer::Get().Cleanup();
}
