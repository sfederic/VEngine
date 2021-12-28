#include <Windows.h>
#include "imgui.h"
#include "ImGuizmo.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "DebugMenu.h"
#include "Editor.h"
#include "Render/Renderer.h"
#include "Render/PipelineObjects.h"
#include "TransformGizmo.h"
#include "Core.h"
#include "Profile.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Actors/IActorSystem.h"
#include "Components/Component.h"
#include "Components/MeshComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "UI/UISystem.h"
#include "Commands/CommandSystem.h"
#include "Commands/ICommand.h"
#include "Physics/Raycast.h"
#include "Input.h"
#include "World.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "SystemCache.h"
#include "System.h"
#include "Actors/Game/Player.h"
#include "Gameplay/Intuition.h"
#include "Console.h"
#include "Physics/PhysicsSystem.h"

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
	ImGui_ImplDX11_Init(renderer.device, renderer.context);
}

void DebugMenu::Tick(float deltaTime)
{
	//Start ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderNotifications(deltaTime);

	//ImGuizmo has to be called here, it's part of ImGui
	transformGizmo.Tick();

	RenderFPSMenu(deltaTime);
	RenderGPUMenu();
	RenderProfileMenu();
	RenderSnappingMenu();
	RenderActorProps();
	RenderCommandsMenu();
	RenderActorInspectMenu();
	RenderWorldStats();
	RenderGameInstanceData();
	RenderMemoryMenu();
	RenderActorSystemMenu();
	RenderComponentSystemMenu();
	RenderIntuitionsMenu();
	RenderConsoleCommandsMenu();
	RenderSkeletonViewMenu();
	RenderCoreMenu();

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DebugMenu::Cleanup()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugMenu::AddNotification(const std::wstring note)
{
	debugNotifications.push_back(DebugNotification(note));
}

void DebugMenu::RenderActorProps()
{
	if (!propsMenuOpen)
	{
		return;
	}

	if (worldEditor.pickedActor == nullptr)
	{
		return;
	}


	ImGui::Begin("Actor Properties");

	//Iterate over actor props
	for (auto props : worldEditor.pickedActor->GetAllProps())
	{
		IterateOverProperties(props);
	}

	ImGui::End();
}

void DebugMenu::IterateOverProperties(Properties& props)
{
	ImGui::Text(props.title.c_str());

	for (auto prop : props.propMap)
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
			ImGui::Text(texture->filename.c_str());
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

	for (auto mesh : MeshComponent::system.components)
	{
		totalVerticesInWorld += mesh->meshDataProxy->vertices->size();
	}

	for (auto instanceMesh : InstanceMeshComponent::system.components)
	{
		totalVerticesInWorld += instanceMesh->meshDataProxy->vertices->size();
	}

	ImGui::Text("Vertex Count: %d", totalVerticesInWorld);

	//Num of actors
	uint64_t actorCount = 0;
	for (auto actorSystem : world.activeActorSystems)
	{
		actorCount += actorSystem->GetActors().size();
	}

	ImGui::Text("Active Actors: %d", actorCount);

	//Num of components
	uint64_t componentCount = 0;
	for (auto componentSystem : world.activeComponentSystems)
	{
		componentCount += componentSystem->GetComponents().size();
	}

	ImGui::Text("Active Components: %d", componentCount);


	ImGui::End();
}

void DebugMenu::RenderGameInstanceData()
{
	if (!gameInstaceMenuOpen)
	{
		return;
	}

	ImGui::Begin("Game Instance Data");

	ImGui::Checkbox("Use GameSaves", &GameInstance::useGameSaves);
	ImGui::InputInt("Current Hour", &GameInstance::currentHour);
	ImGui::InputInt("Current Minute", &GameInstance::currentMinute);
	if (ImGui::Button("Reset Instance Data"))
	{
		GameInstance::ResetTime();
	}

	ImGui::End();
}

void DebugMenu::RenderMemoryMenu()
{
	if (!memoryMenuOpen)
	{
		return;
	}

	ImGui::Begin("Memory");

	for (auto systemIt : *systemCache.nameToSystemMap)
	{
		System* system = systemIt.second;
		ImGui::Text(system->name.c_str());
		switch (system->systemState)
		{
		case SystemStates::Loaded:
			ImGui::Text("Loaded");
			break;

		case SystemStates::Unloaded:
			ImGui::Text("Unloaded");
			break;
		}
	}

	ImGui::End();
}

void DebugMenu::RenderActorSystemMenu()
{
	if (!actorSystemMenuOpen) return;

	ImGui::Begin("Actor Systems");

	for (auto actorSystem : world.activeActorSystems)
	{
		ImGui::Text("Name: %s |", actorSystem->name.c_str());
		ImGui::SameLine();
		ImGui::Text("Actor Count: %d", actorSystem->GetNumActors());
	}

	ImGui::End();
}

void DebugMenu::RenderComponentSystemMenu()
{
	if (!componentSystemMenuOpen) return;

	ImGui::Begin("Component Systems");

	for (auto componentSystem : world.activeComponentSystems)
	{
		ImGui::Text("Name: %s |", componentSystem->name.c_str());
		ImGui::SameLine();
		ImGui::Text("Actor Count: %d", componentSystem->GetNumComponents());
	}

	ImGui::End();
}

void DebugMenu::RenderIntuitionsMenu()
{
	if (!intuitionsMenuOpen) return;

	auto player = GameUtils::GetPlayer();

	ImGui::Begin("Intuitions");

	for (auto& intuition : player->intuitions)
	{
		Intuition* i = intuition.second;

		ImGui::Text("Name: %s", i->name.c_str());
		ImGui::Text("Desc: %s", i->description.c_str());
		ImGui::Text("Func: %s", i->conditionFuncName.c_str());
		ImGui::Text("From World: %s", i->worldAquiredFrom.c_str());
		ImGui::Text("From Actor: %s", i->actorAquiredFrom.c_str());
		ImGui::Text("Hour: %d | Minute: %d", i->hourAquired, i->minuteAquired);

		ImGui::NewLine();
	}

	ImGui::End();
}

void DebugMenu::RenderConsoleCommandsMenu()
{
	if (!consoleCommandsMenuOpen) return;

	ImGui::Begin("Console Commands");

	for (auto& command : console.executeMap)
	{
		ImGui::Text("%S", command.first.c_str());
	}

	ImGui::End();
}

void DebugMenu::RenderSkeletonViewMenu()
{
	if (!skeletonViewMenuOpen) return;

	ImGui::Begin("Skeleton View");

	auto picked = worldEditor.pickedActor;
	if (picked)
	{
		auto meshes = picked->GetComponentsOfType<MeshComponent>();
		for (auto mesh : meshes)
		{
			ImGui::Text("Mesh: %s", mesh->meshComponentData.filename.c_str());

			//Debug select animation clip to play via buttons
			for (auto& animation : mesh->GetSkeleton()->animations)
			{
				std::string animationName = animation.first;
				if (ImGui::Button(animationName.c_str()))
				{
					mesh->GetSkeleton()->currentAnimation = animationName.c_str();
					mesh->currentAnimationTime = 0.f;
				}
			}

			for (auto& joint : mesh->meshDataProxy->skeleton->joints)
			{
				ImGui::Text("Joint: %s ", joint.name.c_str());
				ImGui::SameLine();
				ImGui::Text("Index: %d", joint.index);
				ImGui::SameLine();
				ImGui::Text("Parent Index: %d", joint.parentIndex);
			}
		}
	}

	ImGui::End();
}

void DebugMenu::RenderPhysicsMenu()
{
	if (!physicsMenuOpen) return;

	ImGui::Begin("Rigid Bodies");

	for (auto& rigidActorIt : physicsSystem.rigidActorMap)
	{
		PxRigidActor* rigidActor = rigidActorIt.second;
	}

	ImGui::End();
}

void DebugMenu::RenderCoreMenu()
{
	if (!coreMenuOpen) return;

	ImGui::Begin("Core Engine Variables");
	ImGui::InputFloat("TimeScale", &Core::timeScale);
	ImGui::End();
}

//Handle notifications (eg. "Shaders recompiled", "ERROR: Not X", etc)
void DebugMenu::RenderNotifications(float deltaTime)
{
	constexpr float textOffsetX = 20.f;
	constexpr float notificationLifetime = 3.0f;

	uiSystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	for (int i = 0; i < debugNotifications.size(); i++)
	{
		if (debugNotifications[i].timeOnScreen < notificationLifetime)
		{
			debugNotifications[i].timeOnScreen += deltaTime;

			const float notificationOffsetY = 20.f * i;
			uiSystem.d2dRenderTarget->DrawTextA(debugNotifications[i].text.c_str(), debugNotifications[i].text.size(), uiSystem.textFormat,
				{ 0.f, notificationOffsetY, 1000.f, 1000.f }, uiSystem.debugBrushText);
		}
		else
		{
			debugNotifications.erase(debugNotifications.begin() + i);
		}
	}
}

void DebugMenu::RenderFPSMenu(float deltaTime)
{
	if (fpsMenuOpen)
	{
		ImGui::Begin("FPS");

		ImGui::Text("FPS: %d", Core::finalFrameCount);
		ImGui::Text("GPU Render Time: %f", renderer.frameTime);
		ImGui::Text("Delta Time (ms): %f", deltaTime);
		ImGui::Text("Time Since Startup: %f", Core::timeSinceStartup);

		ImGui::End();
	}
}

void DebugMenu::RenderGPUMenu()
{
	if (gpuMenuOpen)
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
	}
}

void DebugMenu::RenderProfileMenu()
{
	if (profileMenuOpen)
	{
		ImGui::Begin("Profiler Time Frames");

		for (auto& timeFrame : Profile::timeFrames)
		{
			ImGui::Text(timeFrame.first.c_str());
			double time = timeFrame.second.GetAverageTime();
			ImGui::Text(std::to_string(time).c_str());
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
		Ray ray;
		if (RaycastFromScreen(ray))
		{
			Actor* actor = ray.hitActor;
			if (actor)
			{
				ImGui::Begin("Actor Inspect");
				ImGui::SetWindowPos(ImVec2(editor->viewportMouseX, editor->viewportMouseY));

				ImGui::Text("Name: %s", actor->name.c_str());
				ImGui::Text("System: %s", actor->actorSystem->name.c_str());
				ImGui::Text("SystemIndex: %d", actor->index);
				ImGui::Text("Active: %d", actor->IsActive());
				ImGui::Text("UID: %u", actor->uid);
				ImGui::Text("Num Components: %d", actor->components.size());
				ImGui::Text("Parent Actor: %s", actor->parent != nullptr ? actor->parent->name.c_str() : "None");
				ImGui::Text("Num Children: %d", actor->children.size());

				ImGui::End();
			}
		}
	}
}
