#include "vpch.h"
#include <filesystem>
#include <Windows.h>
#include "imgui.h"
#include "ImGuizmo.h"
#include "CutsceneSequencer.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "DebugMenu.h"
#include "Core/Input.h"
#include "Editor.h"
#include "Render/Renderer.h"
#include "Render/PipelineObjects.h"
#include "Render/TextureSystem.h"
#include "TransformGizmo.h"
#include "Core/Core.h"
#include "Core/Profile.h"
#include "Core/WorldEditor.h"
#include "Actors/Actor.h"
#include "Actors/IActorSystem.h"
#include "Components/Component.h"
#include "Components/MeshComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UI/UISystem.h"
#include "UI/Layout.h"
#include "Commands/CommandSystem.h"
#include "Commands/ICommand.h"
#include "Physics/Raycast.h"
#include "Core/World.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Core/SystemCache.h"
#include "Core/System.h"
#include "Actors/Game/Player.h"
#include "Console.h"
#include "Physics/PhysicsSystem.h"
#include "Render/RenderUtils.h"
#include "Render/Texture2D.h"
#include "Render/MaterialSystem.h"
#include "Render/Material.h"

DebugMenu debugMenu;

void DebugMenu::Init()
{
	//IMGUI setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.Fonts->AddFontFromFileTTF("Fonts/OpenSans.ttf", 20);

	//Imgui has an .ini file to save previous ui positions and values.
	//Setting this to null removes this initial setup.
	io.IniFilename = nullptr;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init((HWND)editor->windowHwnd);
	ImGui_ImplDX11_Init(RenderUtils::device, RenderUtils::context);
}

void DebugMenu::Tick(float deltaTime)
{
	if (!Core::isImGUIEnabled) return;

	//Start ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderNotifications(deltaTime);

	//ImGuizmo has to be called here, it's part of ImGui
	transformGizmo.Tick();

	if (cutsceneSequencerOpen)
	{
		cutsceneSequencer.UITick(deltaTime);
	}

	RenderFPSMenu(deltaTime);
	RenderGPUMenu();
	RenderProfileMenu();
	RenderSnappingMenu();
	RenderActorProps();
	RenderCommandsMenu();
	RenderActorInspectMenu();
	RenderWorldStats();
	RenderGameInstanceData();
	RenderActorSystemMenu();
	RenderComponentSystemMenu();
	RenderSkeletonViewMenu();
	RenderSkeletalAnimationMenu();
	RenderCoreMenu();
	RenderParticleMenu();
	RenderTexturePlacementMenu();
	RenderConsoleCommandsMenu();
	RenderWorldMenu();
	RenderMaterialPlacementMenu();
	RenderActorSystemSpawnMenu();

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	hasMouseFocus = ImGui::GetIO().WantCaptureMouse;
}

void DebugMenu::Cleanup()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugMenu::AddNotification(const std::wstring note)
{
	debugNotifications.emplace_back(DebugNotification(note));
}

void DebugMenu::RenderActorProps()
{
	if (!propsMenuOpen)
	{
		return;
	}

	if (WorldEditor::GetPickedActor() == nullptr)
	{
		return;
	}


	ImGui::Begin("Actor Properties");

	//Iterate over actor props
	for (auto& props : WorldEditor::GetPickedActor()->GetAllProps())
	{
		IterateOverProperties(props);
	}

	ImGui::End();
}

void DebugMenu::IterateOverProperties(Properties& props)
{
	ImGui::Text(props.title.c_str());

	for (auto& prop : props.propMap)
	{
		const std::string& name = prop.first;

		if (props.CheckType<bool>(name))
		{
			ImGui::Checkbox(name.c_str(), props.GetData<bool>(name));
		}
		else if (props.CheckType<int>(name))
		{
			ImGui::InputInt(name.c_str(), props.GetData<int>(name));
		}
		else if (props.CheckType<float>(name))
		{
			ImGui::InputFloat(name.c_str(), props.GetData<float>(name));
		}
		else if (props.CheckType<XMFLOAT3>(name))
		{
			DirectX::XMFLOAT3* xmfloat3 = props.GetData<XMFLOAT3>(name);
			float* f3[3] = { &xmfloat3->x, &xmfloat3->y, &xmfloat3->z };
			ImGui::InputFloat3(name.c_str(), *f3);
		}		
		else if (props.CheckType<XMFLOAT4>(name))
		{
			DirectX::XMFLOAT4* xmfloat4 = props.GetData<XMFLOAT4>(name);
			float* f4[4] = { &xmfloat4->x, &xmfloat4->y, &xmfloat4->z, &xmfloat4->w };
			ImGui::InputFloat4(name.c_str(), *f4);
		}		
		else if (props.CheckType<XMFLOAT2>(name))
		{
			DirectX::XMFLOAT2* xmfloat2 = props.GetData<XMFLOAT2>(name);
			float* f2[2] = { &xmfloat2->x, &xmfloat2->y };
			ImGui::InputFloat2(name.c_str(), *f2);
		}
		else if (props.CheckType<std::string>(name))
		{
			std::string* str = props.GetData<std::string>(name);
			ImGui::InputText(name.c_str(), str->data(), str->size());
		}		
		else if (props.CheckType<Texture2D>(name))
		{
			Texture2D* texture = props.GetData<Texture2D>(name);
			ImGui::Text(name.c_str());
			ImGui::SameLine();
			ImGui::Text(texture->GetFilename().c_str());
		}
		else if (props.CheckType<Transform>(name))
		{
			Transform* transform = props.GetData<Transform>(name);

			float* position[3] = { &transform->position.x, &transform->position.y, &transform->position.z };
			ImGui::InputFloat3("Position", *position);

			float* scale[3] = { &transform->scale.x, &transform->scale.y, &transform->scale.z };
			ImGui::InputFloat3("Scale", *scale);

			float* rotation[4] = { &transform->rotation.x, &transform->rotation.y, &transform->rotation.z };
			ImGui::InputFloat4("Rotation", *rotation);
		}
	}
}

void DebugMenu::RenderCommandsMenu()
{
	if (!commandsMenuOpen) return;

	ImGui::Begin("Commands");

	if (ImGui::BeginListBox("First"))
	{
		unsigned int cmdCount = 0;

		for (ICommand* command : commandSystem.commands)
		{
			std::string name = command->name + std::to_string(cmdCount);

			if (ImGui::Selectable(name.c_str()))
			{
				commandSystem.WindToCommand(cmdCount);
			}

			cmdCount++;
		}
		ImGui::EndListBox();
	}

	if (ImGui::Button("Clear Commands"))
	{
		commandSystem.Reset();
	}

	ImGui::End();
}

void DebugMenu::RenderWorldStats()
{
	if (!worldStatsMenuOpen)
	{
		return;
	}

	ImGui::Begin("World Stats");

	//Num of vertices in world
	uint64_t totalVerticesInWorld = 0;

	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		totalVerticesInWorld += mesh->meshDataProxy.vertices->size();
	}

	for (auto& instanceMesh : InstanceMeshComponent::system.GetComponents())
	{
		totalVerticesInWorld += instanceMesh->meshDataProxy.vertices->size();
	}

	ImGui::Text("Vertex Count: %d", totalVerticesInWorld);

	//Num of actors
	uint64_t actorCount = 0;
	for (auto actorSystem : World::activeActorSystems)
	{
		actorCount += actorSystem->GetActorsAsBaseClass().size();
	}

	ImGui::Text("Active Actors: %d", actorCount);

	//Num of components
	uint64_t componentCount = 0;
	for (auto componentSystem : World::activeComponentSystems)
	{
		componentCount += componentSystem->GetNumComponents();
	}

	ImGui::Text("Active Components: %d", componentCount);

	ImGui::End();
}

void DebugMenu::RenderGameInstanceData()
{
	if (!gameInstaceMenuOpen) return;

	ImGui::Begin("Game Instance Data");
	ImGui::End();
}

void DebugMenu::RenderActorSystemMenu()
{
	if (!actorSystemMenuOpen) return;

	ImGui::Begin("Actor Systems");

	for (auto actorSystem : World::activeActorSystems)
	{
		ImGui::Text("Name: %s |", actorSystem->GetName().c_str());
		ImGui::SameLine();
		ImGui::Text("Actor Count: %d", actorSystem->GetNumActors());
	}

	ImGui::End();
}

void DebugMenu::RenderComponentSystemMenu()
{
	if (!componentSystemMenuOpen) return;

	ImGui::Begin("Component Systems");

	for (auto componentSystem : World::activeComponentSystems)
	{
		ImGui::Text("Name: %s |", componentSystem->name.c_str());
		ImGui::SameLine();
		ImGui::Text("Actor Count: %d", componentSystem->GetNumComponents());
	}

	ImGui::End();
}

void DebugMenu::RenderSkeletonViewMenu()
{
	if (!skeletonViewMenuOpen) return;

	ImGui::Begin("Skeleton View");

	auto pickedActor = WorldEditor::GetPickedActor();
	if (pickedActor)
	{
		auto skeletalMeshes = pickedActor->GetComponentsOfType<SkeletalMeshComponent>();
		for (auto skeletalMesh : skeletalMeshes)
		{
			ImGui::Text("Skeleton: %s", skeletalMesh->meshComponentData.filename.c_str());

			for (auto& joint : skeletalMesh->GetSkeleton().joints)
			{
				ImGui::Text("Joint: %s ", joint.name);
				ImGui::SameLine();
				ImGui::Text("Index: %d", joint.index);
				ImGui::SameLine();
				ImGui::Text("Parent Index: %d", joint.parentIndex);
			}
		}
	}

	ImGui::End();
}

void DebugMenu::RenderSkeletalAnimationMenu()
{
	if (!skeletalAnimationMenuOpen) return;

	ImGui::Begin("Animation Menu");

	auto picked = WorldEditor::GetPickedActor();
	if (picked)
	{
		auto skeletalMeshes = picked->GetComponentsOfType<SkeletalMeshComponent>();
		for (auto skeletalMesh : skeletalMeshes)
		{
			ImGui::Text("SkeletalMesh: %s", skeletalMesh->meshComponentData.filename.c_str());
			ImGui::Text("Current Animation: %s", skeletalMesh->GetCurrentAnimatonName().c_str());

			if (!skeletalMesh->GetCurrentAnimatonName().empty())
			{
				ImGui::Text("Time: %f/%f",
					skeletalMesh->GetCurrentAnimationTime(),
					skeletalMesh->GetCurrentAnimation().GetFinalTime());
			}

			static float animationSpeed = 1.f;
			ImGui::InputFloat("Animation Speed", &animationSpeed);
			skeletalMesh->SetAnimationSpeed(animationSpeed);

			//Debug select animation clip to play via buttons
			for (auto& animation : skeletalMesh->GetSkeleton().animations)
			{
				std::string animationName = animation.first;
				if (!animationName.empty())
				{
					if (ImGui::Button(animationName.c_str()))
					{
						skeletalMesh->PlayAnimation(animationName, animationSpeed);
						skeletalMesh->ResetAnimationTime();
					}
				}
			}
		}
	}

	ImGui::End();
}

void DebugMenu::RenderPhysicsMenu()
{
	if (!physicsMenuOpen) return;

	ImGui::Begin("Rigid Bodies");

	for (auto rigidActor : PhysicsSystem::GetRigidActors())
	{
	}

	ImGui::End();
}

void DebugMenu::RenderCoreMenu()
{
	if (!coreMenuOpen) return;

	ImGui::Begin("Core Engine Variables");
	ImGui::InputFloat("TimeScale", &Core::timeScale);
	ImGui::Checkbox("ImGui Enabled", &Core::isImGUIEnabled);
	ImGui::End();
}

//@Todo: this function needs a 'getallcomponentsoftype()' to be able to work properly and display
//number of particle systems and particles in world
void DebugMenu::RenderParticleMenu()
{
	if (!particleMenuOpen) return;

	ImGui::Begin("Particles");

	ImGui::End();
}

void DebugMenu::RenderConsoleCommandsMenu()
{
	if (consoleCommandsMenuOpen)
	{
		ImGui::Begin("Console Commands");

		for (auto& consoleMapPair : Console::executeMap)
		{
			std::wstring functionName = consoleMapPair.first;
			std::string functionDescription = consoleMapPair.second.second;

			ImGui::Text("%S | %s", functionName.c_str(), functionDescription.c_str());
		}

		ImGui::End();
	}
}

void DebugMenu::RenderWorldMenu()
{
	if (!worldMenuOpen) return;

	ImGui::Begin("Worlds");

	for (auto& worldEntry : std::filesystem::directory_iterator("WorldMaps"))
	{
		const std::string worldFilename = worldEntry.path().filename().string();
		if (ImGui::Button(worldFilename.c_str()))
		{
			GameUtils::LoadWorldDeferred(worldFilename);
		}
	}

	ImGui::End();
}

void DebugMenu::RenderActorSystemSpawnMenu()
{
	if (Input::GetKeyHeld(Keys::Ctrl))
	{
		if (Input::GetKeyUp(Keys::T))
		{
			actorSystemSpawnMenuOpen = !actorSystemSpawnMenuOpen;
		}
	}

	if (actorSystemSpawnMenuOpen)
	{
		ImGui::Begin("Actor System Spawn Menu");
		ImGui::SetWindowSize(ImVec2(250.f, 100.f));
		static char systemName[512];
		if (ImGui::InputText("System", systemName, sizeof(systemName)))
		{
			std::string str = systemName;
			auto system = ActorSystemCache::Get().GetSystem(str);
			if (system)
			{
				WorldEditor::SetSpawnSystem(system);
				Log("Actor Spawn System set as %s", str.c_str());
			}
		}
		ImGui::End();
	}
}

//Handle viewport notifications (e.g. "Shaders recompiled", "ERROR: Not X", etc.)
void DebugMenu::RenderNotifications(float deltaTime)
{
	constexpr float textOffsetX = 20.f;
	constexpr float notificationLifetime = 3.0f;

	for (int i = 0; i < debugNotifications.size(); i++)
	{
		if (debugNotifications[i].timeOnScreen < notificationLifetime)
		{
			debugNotifications[i].timeOnScreen += deltaTime;

			const float notificationOffsetY = 20.f * i;
			const auto textLayout = Layout({ 0.f, notificationOffsetY, 1000.f, 1000.f });
			UISystem::TextDraw(debugNotifications[i].text, textLayout, DWRITE_TEXT_ALIGNMENT_JUSTIFIED, Colours::Green);
		}
		else
		{
			debugNotifications.erase(debugNotifications.begin() + i);
		}
	}
}

void DebugMenu::RenderTexturePlacementMenu()
{
	if (WorldEditor::texturePlacement)
	{
		ImGui::Begin("Texture Placement = ON: (Place selected texture with left click)");
		ImGui::Text("Selected Texture: %S", TextureSystem::selectedTextureInEditor.c_str());
		ImGui::End();
	}
}

void DebugMenu::RenderMaterialPlacementMenu()
{
	if (WorldEditor::materialPlacement)
	{
		ImGui::Begin("Material Placement = ON: (Place selected material with left click)");
		ImGui::Text("Selected Material: %s", MaterialSystem::selectedMaterialInEditor.c_str());
		ImGui::End();
	}
}

void DebugMenu::RenderFPSMenu(float deltaTime)
{
	if (fpsMenuOpen)
	{
		ImGui::Begin("FPS");

		ImGui::Text("FPS: %d", Core::finalFrameCount);
		ImGui::Text("GPU Render Time: %f", Renderer::frameTime);
		ImGui::Text("Delta Time (ms): %f", deltaTime);
		ImGui::Text("Time Since Startup: %f", Core::timeSinceStartup);

		ImGui::End();
	}
}

void DebugMenu::RenderGPUMenu()
{
	/*if (gpuMenuOpen)
	{
		ImGui::Begin("GPU Info");

		DXGI_ADAPTER_DESC1 adapterDesc = renderer.gpuAdaptersDesc.front();

		ImGui::Text("Device: %ls", adapterDesc.Description);
		ImGui::Text("System Memory: %zu", adapterDesc.DedicatedSystemMemory);
		ImGui::Text("Video Memory: %zu", adapterDesc.DedicatedVideoMemory);
		ImGui::Text("Shared System Memory: %zu", adapterDesc.SharedSystemMemory);
		ImGui::Spacing();

		static bool showAllDevices;
		if (!showAllDevices && ImGui::Button("Show all Devices"))
		{
			showAllDevices = true;
		}
		else if (showAllDevices && ImGui::Button("Hide all Devices"))
		{
			showAllDevices = false;
		}

		if (showAllDevices)
		{
			for (int i = 1; i < renderer.gpuAdaptersDesc.size(); i++)
			{
				ImGui::Text("Device: %ls", renderer.gpuAdaptersDesc[i].Description);
				ImGui::Text("System Memory: %zu", renderer.gpuAdaptersDesc[i].DedicatedSystemMemory);
				ImGui::Text("Video Memory: %zu", renderer.gpuAdaptersDesc[i].DedicatedVideoMemory);
				ImGui::Text("Shared System Memory: %zu", renderer.gpuAdaptersDesc[i].SharedSystemMemory);
				ImGui::Spacing();
			}
		}

		ImGui::End();
	}*/
}

void DebugMenu::RenderProfileMenu()
{
	if (profileMenuOpen)
	{
		ImGui::Begin("Profiler Time Frames");

		//Sort TimeFrames in order of average time
		std::map<double, std::string> averageTimes;
		for (auto& [functionName, timeFrame] : Profile::timeFrames)
		{
			averageTimes.emplace(timeFrame.GetAverageTime(), functionName);
		}

		for (auto averageTimesIt = averageTimes.rbegin(); averageTimesIt != averageTimes.rend(); averageTimesIt++)
		{
			ImGui::Text(averageTimesIt->second.c_str());
			ImGui::Text(std::to_string(averageTimesIt->first).c_str());
		}

		ImGui::End();
	}
}

void DebugMenu::RenderSnappingMenu()
{
	if (snapMenuOpen)
	{
		ImGui::Begin("Snapping");

		ImGui::InputFloat("Translation", &transformGizmo.translateSnapValues[0]);
		transformGizmo.translateSnapValues[1] = transformGizmo.translateSnapValues[0];
		transformGizmo.translateSnapValues[2] = transformGizmo.translateSnapValues[0];

		ImGui::InputFloat("Rotation", &transformGizmo.rotationSnapValues[0]);
		transformGizmo.rotationSnapValues[1] = transformGizmo.rotationSnapValues[0];
		transformGizmo.rotationSnapValues[2] = transformGizmo.rotationSnapValues[0];

		ImGui::InputFloat("Scale", &transformGizmo.scaleSnapValues[0]);
		transformGizmo.scaleSnapValues[1] = transformGizmo.scaleSnapValues[0];
		transformGizmo.scaleSnapValues[2] = transformGizmo.scaleSnapValues[0];

		if (transformGizmo.currentTransformMode == ImGuizmo::MODE::LOCAL)
		{
			ImGui::Text("LOCAL");
		}
		else if (transformGizmo.currentTransformMode == ImGuizmo::MODE::WORLD)
		{
			ImGui::Text("WORLD");
		}

		ImGui::End();
	}
}

//Stole this from the Fledge Engine https://www.youtube.com/watch?v=WjPiJn9dkxs
//Works by hovering a menu over the current mouse over'd actor.
void DebugMenu::RenderActorInspectMenu()
{
	if (actorInspectMenuOpen)
	{
		HitResult ray;
		if (RaycastFromScreen(ray))
		{
			Actor* actor = ray.hitActor;
			if (actor)
			{
				ImGui::Begin("Actor Inspect");
				ImGui::SetWindowPos(ImVec2(editor->viewportMouseX, editor->viewportMouseY));

				ImGui::Text("Name: %s", actor->GetName().c_str());
				ImGui::Text("System: %s", actor->GetActorSystem()->GetName().c_str());
				ImGui::Text("SystemIndex: %d", actor->GetSystemIndex());
				ImGui::Text("Active: %d", actor->IsActive());
				ImGui::Text("UID: %u", actor->GetUID());
				ImGui::Text("Num Components: %d", actor->GetAllComponents().size());

				ImGui::End();
			}
		}
	}
}
