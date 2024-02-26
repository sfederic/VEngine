#include "vpch.h"
#include "Engine.h"
#include "Asset/FBXLoader.h"
#include "Audio/AudioSystem.h"
#include "Commands/CommandSystem.h"
#include "Components/MeshComponent.h"
#include "Core/Camera.h"
#include "Core/Core.h"
#include "Core/FileSystem.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/Profile.h"
#include "Core/PropertyTypes.h"
#include "Core/Timer.h"
#include "Core/World.h"
#include "Core/WorldEditor.h"
#include "Editor/Console.h"
#include "Editor/DebugMenu.h"
#include "Editor/Editor.h"
#include "Gameplay/WorldFunctions.h"
#include "Physics/PhysicsSystem.h"
#include "Render/Material.h"
#include "Render/MaterialSystem.h"
#include "Render/MeshComponentFramePacket.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"
#include "Render/SpriteSystem.h"
#include "UI/UISystem.h"
#include <future>
#include <thread>

void PackFramePacketData();
void RunSimulation();
void RunRendering();

void ClearLog()
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

	editor->Init(argc, argv);
	auto rendererInit = std::async(std::launch::async, []() { Renderer::Init(editor->windowHwnd, editor->GetViewportWidth(), editor->GetViewportHeight()); });

	rendererInit.wait();
	MaterialSystem::Init();

	auto debugMenuInit = std::async(std::launch::async, []() { debugMenu.Init(); });
	auto uiInit = std::async(std::launch::async, []() { UISystem::Init(Renderer::GetSwapchain()); });

	audioInit.wait();
	physicsInit.wait();
	fbxInit.wait();
	uiInit.wait();

	World::Init();

	editor->UpdateWorldList();
	editor->UpdateSystemsList();

	debugMenuInit.wait();
	consoleInit.wait();

	const double endTime = Profile::QuickEnd(startTime);
	Log("Startup took: %f seconds", endTime);
}

void Engine::TickSystems(float deltaTime)
{
	editor->Tick();
	Core::Tick();
	CommandSystem::Get().Tick();
	AudioSystem::Tick();

	if (!Core::gameplayOn) //Tick editor camera
	{
		Camera::GetActiveCamera().Tick(deltaTime);
	}

	Timer::Tick(deltaTime);

	WorldEditor::Tick();
	PhysicsSystem::Tick(deltaTime);

	if (Core::gameplayOn && !Core::IsGameWorldPaused())
	{
		World::TickAllActorSystems(deltaTime);
		World::TickAllComponentSystems(deltaTime);
	}
}

void Engine::MainLoop()
{
	while (Core::mainLoop)
	{
		std::thread simulationThread(RunSimulation);
		std::thread renderThread(RunRendering);

		simulationThread.join();
		renderThread.join();
	}
}

void Engine::Render(float deltaTime)
{
	Renderer::Render();
	Renderer::RenderParticleEmitters();

	Console::InputTick();

	UISystem::BeginDraw();
	UISystem::DrawAllWidgets(deltaTime);

	Renderer::RenderSpritesInScreenSpace();

	Console::Tick();
	debugMenu.Tick(deltaTime);
	UISystem::EndDraw();

	Renderer::Present();
}

void Engine::Cleanup()
{
	PhysicsSystem::Cleanup();
	ShaderSystem::ClearShaders();
	debugMenu.Cleanup();
	UISystem::Cleanup();
}

void PackFramePacketData()
{
	Profile::Start();

	std::vector<MeshComponentFramePacket> meshPackets;

	const auto meshes = MeshComponent::SortMeshComponentsByDistance();
	for (const auto& mesh : meshes)
	{
		if (!mesh->IsVisible() || !mesh->IsActive())
		{
			continue;
		}

		const Material& mat = mesh->GetMaterial();

		MeshComponentFramePacket meshPacket = {};
		meshPacket.worldMatrix = mesh->GetWorldMatrix();
		meshPacket.blendState = mesh->GetBlendState();
		meshPacket.rastState = mesh->GetRastState();
		meshPacket.vertexBuffer = mesh->GetVertexBuffer();
		meshPacket.sampler = *mat.sampler;
		meshPacket.shader = *mat.shader;
		meshPacket.texture = *mat.texture;
		meshPacket.materialShaderData = mat.materialShaderData;
		meshPacket.textureData = mat.textureData;
		meshPacket.uvOffsetSpeed = mat.uvOffsetSpeed;
		meshPacket.uvRotationSpeed = mat.uvRotationSpeed;
		meshPacket.vertexCount = mesh->meshDataProxy.vertices.size();

		meshPackets.emplace_back(meshPacket);
	}

	Renderer::PassInMeshComponentFramePackets(meshPackets);

	Profile::End();
}

void RunSimulation()
{
	Engine::TickSystems(Core::GetDeltaTime());
	PackFramePacketData();
	World::DestroyAllDeferredActors();
	FileSystem::DeferredWorldLoad();
	Input::Reset();
}

void RunRendering()
{
	Renderer::Tick();
	Engine::Render(Core::GetDeltaTime());
	SpriteSystem::Reset();
	Renderer::ResizeSwapchain();
}
