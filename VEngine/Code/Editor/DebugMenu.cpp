#include "vpch.h"
#include <filesystem>
#include <Windows.h>
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "DebugMenu.h"
#include "Core/VMath.h"
#include "Editor.h"
#include "Render/Renderer.h"
#include "Render/TextureSystem.h"
#include "TransformGizmo.h"
#include "Core/Core.h"
#include "Core/Profile.h"
#include "Core/WorldEditor.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UI/UISystem.h"
#include "UI/UVPaintWidget.h"
#include "UI/Layout.h"
#include "Physics/Raycast.h"
#include "Core/World.h"
#include "Gameplay/GameUtils.h"
#include "Console.h"
#include "Physics/PhysicsSystem.h"
#include "Render/Texture2D.h"
#include "Render/MaterialSystem.h"
#include "Render/Material.h"
#include "Particle/ParticleEmitter.h"
#include "Animation/Skeleton.h"
#include "Asset/AssetSystem.h"
#include "Editor/Sequencer/Sequencer.h"

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
	ImGui_ImplWin32_Init((HWND)Editor::Get().windowHwnd);
	ImGui_ImplDX11_Init(&Renderer::GetDevice(), &Renderer::GetDeviceContext());
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

	if (sequencerOpen)
	{
		gSequencer.Render();
	}

	RenderFPSMenu(deltaTime);
	RenderGPUMenu();
	RenderProfileMenu();
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
	RenderVertexPaintMenu();
	RenderUVMenu();
	RenderUVPaintMenu();
	RenderWidgetDetailsMenu();

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

void DebugMenu::ClearAllStaticNotifications()
{
	for (int i = 0; i < debugNotifications.size(); i++)
	{
		if (debugNotifications[i].staticNotification)
		{
			debugNotifications.erase(debugNotifications.begin() + i);
		}
	}
}

void DebugMenu::AddNotification(const std::wstring note)
{
	debugNotifications.emplace_back(DebugNotification(note));
}

void DebugMenu::AddStaticNotification(std::wstring note)
{
	DebugNotification dn(note);
	dn.staticNotification = true;
	debugNotifications.emplace_back(dn);
}

void DebugMenu::CreateUVPaintWidget()
{
	if (uvPaintWidget == nullptr)
	{
		uvPaintWidget = UISystem::CreateWidget<UVPaintWidget>();
	}
}

void DebugMenu::SetUVPaintMenuTextureFilename()
{
	//Needs to be called here so that World::Cleanup() doesn't remove the widget.
	uvPaintMenuTextureFilename = uvPaintWidget->uvPreviewTextureFilename;
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
		totalVerticesInWorld += mesh->meshDataProxy.vertices.size();
	}

	for (auto& instanceMesh : InstanceMeshComponent::system.GetComponents())
	{
		totalVerticesInWorld += instanceMesh->meshDataProxy.vertices.size();
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
		if (actorSystem->GetNumActors() > 0)
		{
			ImGui::Text("%s | ", actorSystem->GetName().c_str());
			ImGui::SameLine();
			ImGui::Text("Count: %d", actorSystem->GetNumActors());
		}
	}

	ImGui::End();
}

void DebugMenu::RenderComponentSystemMenu()
{
	if (!componentSystemMenuOpen) return;

	ImGui::Begin("Component Systems");

	for (auto componentSystem : World::activeComponentSystems)
	{
		if (componentSystem->GetNumComponents() > 0)
		{
			ImGui::Text("%s | ", componentSystem->GetName().c_str());
			ImGui::SameLine();
			ImGui::Text("Count: %d", componentSystem->GetNumComponents());
		}
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
		auto skeletalMeshes = pickedActor->GetComponents<SkeletalMeshComponent>();
		for (auto skeletalMesh : skeletalMeshes)
		{
			ImGui::Text("Skeleton: %s", skeletalMesh->meshComponentData.filename.c_str());

			for (auto& joint : skeletalMesh->GetSkeleton().GetJoints())
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
		auto skeletalMeshes = picked->GetComponents<SkeletalMeshComponent>();
		for (auto skeletalMesh : skeletalMeshes)
		{
			ImGui::Text("SkeletalMesh: %s", skeletalMesh->meshComponentData.filename.c_str());
			ImGui::Text("Current Animation: %s", skeletalMesh->GetCurrentAnimationName().c_str());

			if (!skeletalMesh->GetCurrentAnimationName().empty())
			{
				ImGui::Text("Time: %f/%f",
					skeletalMesh->GetCurrentAnimationTime(),
					skeletalMesh->GetCurrentAnimation().GetFinalTime());
			}

			static float animationSpeed = 1.f;
			ImGui::InputFloat("Animation Speed", &animationSpeed);
			skeletalMesh->SetAnimationSpeed(animationSpeed);

			//Debug select animation clip to play via buttons
			for (auto& animation : skeletalMesh->GetSkeleton().GetAnimations())
			{
				std::string animationName = animation.first;
				if (!animationName.empty())
				{
					if (ImGui::Button(animationName.c_str()))
					{
						skeletalMesh->PlayAnimation(animationName, animationSpeed);
						skeletalMesh->ResetAnimationTime();
					}
					if (ImGui::Button(std::string(animationName + " CrossFade").c_str()))
					{
						skeletalMesh->SetCrossFade(animationName);
					}
				}
			}
		}
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

void DebugMenu::RenderParticleMenu()
{
	if (!particleMenuOpen) return;

	ImGui::Begin("Particles");

	for (auto emitter : World::GetAllComponentsOfType<ParticleEmitter>())
	{
		ImGui::Text(emitter->GetName().c_str());
		ImGui::Text("Particle Count: %u", emitter->GetParticleCount());
		ImGui::NewLine();
	}

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

void DebugMenu::RenderVertexPaintMenu()
{
	if (!vertexPaintMenuOpen) return;

	ImGui::Begin("Vertex Paint Menu");

	if (ImGui::Button("Save Vertex Colours"))
	{
		AssetSystem::WriteOutAllVertexColourData();
	}

	float colour[4] = {
		WorldEditor::vertexPaintColour.x,
		WorldEditor::vertexPaintColour.y,
		WorldEditor::vertexPaintColour.z,
		WorldEditor::vertexPaintColour.w,
	};

	ImGui::ColorPicker4("Vertex Colour", colour);

	WorldEditor::vertexPaintColour.x = colour[0];
	WorldEditor::vertexPaintColour.y = colour[1];
	WorldEditor::vertexPaintColour.z = colour[2];
	WorldEditor::vertexPaintColour.w = colour[3];

	if (ImGui::Button("Load Vertex Colours"))
	{
		AssetSystem::LoadVertexColourDataFromFile();
	}

	if (ImGui::Button("Lock Actor to Paint"))
	{
		WorldEditor::vertexPaintLockActor = WorldEditor::GetPickedActor();
	}

	if (WorldEditor::vertexPaintLockActor != nullptr)
	{
		ImGui::LabelText("Lock Actor: %s", WorldEditor::vertexPaintLockActor->GetName().c_str());
		if (ImGui::Button("Unlock Actor"))
		{
			WorldEditor::vertexPaintLockActor = nullptr;
		}
	}

	auto SetMeshVertexColours = [&](XMFLOAT4 colour)
		{
			for (auto& mesh : MeshComponent::system.GetComponents())
			{
				for (auto& vertex : mesh->GetAllVertices())
				{
					vertex.colour = colour;
				}

				mesh->CreateNewVertexBuffer();
			}
		};

	if (ImGui::Button("Reset all vertex colours in world"))
	{
		SetMeshVertexColours(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	}

	if (ImGui::Button("Fill Selected Actor"))
	{
		auto pickedActor = WorldEditor::GetPickedActor();
		if (pickedActor != nullptr)
		{
			for (auto* mesh : pickedActor->GetComponents<MeshComponent>())
			{
				for (auto& vertex : mesh->GetAllVertices())
				{
					vertex.colour = WorldEditor::vertexPaintColour;
				}

				mesh->CreateNewVertexBuffer();
			}
		}
	}

	if (ImGui::Button("Flood Fill Set"))
	{
		SetMeshVertexColours(XMFLOAT4(colour[0], colour[1], colour[2], 1.f));
	}

	if (ImGui::Button("Flood Fill Multiply"))
	{
		for (auto& mesh : MeshComponent::system.GetComponents())
		{
			const auto fillColour = XMLoadFloat4(&WorldEditor::vertexPaintColour);
			for (auto& vertex : mesh->GetAllVertices())
			{
				auto vertexColour = XMLoadFloat4(&vertex.colour);
				vertexColour *= fillColour;
				XMStoreFloat4(&vertex.colour, vertexColour);
			}

			mesh->CreateNewVertexBuffer();
		}
	}

	if (ImGui::Button("Face Fill Mode"))
	{
		WorldEditor::vertexPaintFaceFillMode = !WorldEditor::vertexPaintFaceFillMode;
	}
	ImGui::SameLine();
	ImGui::Text("%d", WorldEditor::vertexPaintFaceFillMode);

	if (ImGui::Button("Export VMesh"))
	{
		AssetSystem::CreateVMeshFromInWorldMesh();
	}

	ImGui::End();
}

void DebugMenu::RenderUVMenu()
{
	if (!uvMenuOpen) return;

	ImGui::Begin("UV Menu");

	if (WorldEditor::GetPickedActor())
	{
		const auto worldPosition = WorldEditor::GetPickedActor()->GetHomogeneousPositionV();
		int sx = 0, sy = 0;
		VMath::HomogenousWorldPosToScreenSpaceCoords(worldPosition, sx, sy);
		ImGui::SetWindowPos(ImVec2(sx, sy));

		for (const auto mesh : WorldEditor::GetPickedActor()->GetComponents<MeshComponent>())
		{
			auto& materialShaderData = mesh->GetMaterial().GetMaterialShaderData();

			static float uvScale[2];
			uvScale[0] = materialShaderData.uvScale.x;
			uvScale[1] = materialShaderData.uvScale.y;
			if (ImGui::InputFloat2("UV Scale", uvScale))
			{
				materialShaderData.uvScale.x = uvScale[0];
				materialShaderData.uvScale.y = uvScale[1];
			}

			static float uvOffset[2];
			uvOffset[0] = materialShaderData.uvOffset.x;
			uvOffset[1] = materialShaderData.uvOffset.y;
			if (ImGui::InputFloat2("UV Offset", uvOffset))
			{
				materialShaderData.uvOffset.x = uvOffset[0];
				materialShaderData.uvOffset.y = uvOffset[1];
			}

			static float uvRotation;
			uvRotation = materialShaderData.uvRotation;
			if (ImGui::InputFloat("UV Rotation", &uvRotation))
			{
				materialShaderData.uvRotation = uvRotation;
			}
		}
	}

	ImGui::End();
}

void DebugMenu::RenderUVPaintMenu()
{
	if (!uvPaintMenuOpen && uvPaintWidget == nullptr)
	{
		return;
	}
	if (!uvPaintMenuOpen)
	{
		uvPaintWidget->RemoveFromViewport();
		return;
	}
	uvPaintWidget->AddToViewport();

	ImGui::Begin("UV Paint (Left click to paint left tri, Left+Ctrl to paint right tri)");
	if (ImGui::Button("Paint On/off"))
	{
		WorldEditor::uvPaintOn = !WorldEditor::uvPaintOn;
		WorldEditor::uvPaintOn ?
			AddStaticNotification(L"uv paint on") :
			ClearAllStaticNotifications();
	}

	ImGui::InputFloat("Picker Width", &uvPaintWidget->uvPickerWidth);
	ImGui::InputFloat("Picker Height", &uvPaintWidget->uvPickerHeight);

	//ImGui what are you doing use an enum.
	//ImGuiDir is just an int typedef. https://wiki.giderosmobile.com/index.php/ImGui.Core:arrowButton
	if (ImGui::ArrowButton("Left", 0))
	{
		WorldEditor::uvPaintData.uvPaintRotate = UVPaintRotate::Left;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("Right", 1))
	{
		WorldEditor::uvPaintData.uvPaintRotate = UVPaintRotate::Right;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("Up", 2))
	{
		WorldEditor::uvPaintData.uvPaintRotate = UVPaintRotate::Up;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("Down", 3))
	{
		WorldEditor::uvPaintData.uvPaintRotate = UVPaintRotate::Down;
	}
	ImGui::SameLine();
	static std::string paintRotateStr;
	switch (WorldEditor::uvPaintData.uvPaintRotate)
	{
	case UVPaintRotate::Left:
		paintRotateStr = "Left";
		break;
	case UVPaintRotate::Right:
		paintRotateStr = "Right";
		break;
	case UVPaintRotate::Up:
		paintRotateStr = "Up";
		break;
	case UVPaintRotate::Down:
		paintRotateStr = "Down";
		break;
	}
	ImGui::Text(paintRotateStr.c_str());

	if (ImGui::Button("Export Selected Actor .vmesh"))
	{
		AssetSystem::CreateVMeshFromInWorldMesh();
		Log("Exported .vmesh for picked actor UV painting.");
	}

	ImGui::End();
}

//Todo: there's an issue here where because DebugMenu::Tick() is called before Direct2D ends its frame,
//ImGui is drawn underneath Widgets. It's not terrible, still usable but it doesn't look clean.
void DebugMenu::RenderWidgetDetailsMenu()
{
	if (!widgetDetailsMenuOpen)
	{
		return;
	}

	ImGui::Begin("Widget Details");

	const std::vector<Widget*> widgetsInViewport = UISystem::GetAllWidgetsInViewport();

	if (widgetsInViewport.empty())
	{
		ImGui::Text("No Widgets in viewport.");
	}

	const int mouseViewportPosX = Editor::Get().GetViewportMouseX();
	const int mouseViewportPosY = Editor::Get().GetViewportMouseY();

	ImGui::SetWindowPos(ImVec2(mouseViewportPosX, mouseViewportPosY));

	for (Widget* widget : widgetsInViewport)
	{
		widget->DrawDebugRectsForAllActiveLayouts();

		for (const auto& layout : widget->GetAllActiveControlLayouts())
		{
			if (layout.IsPosInLayout(mouseViewportPosX, mouseViewportPosY))
			{
				const std::string widgetName = widget->GetName();
				ImGui::Text(widgetName.c_str());
				break;
			}
		}
	}

	ImGui::End();
}

//Handle viewport notifications (e.g. "Shaders recompiled", "ERROR: Not X", etc.)
void DebugMenu::RenderNotifications(float deltaTime)
{
	constexpr float notificationLifetime = 3.0f;

	for (int i = 0; i < debugNotifications.size(); i++)
	{
		if (debugNotifications[i].timeOnScreen < notificationLifetime)
		{
			if (!debugNotifications[i].staticNotification)
			{
				debugNotifications[i].timeOnScreen += deltaTime;
			}

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
		ImGui::Text("Total Frame Time %f | (60 FPS) %f", Profile::GetTotalFrameTime(), 60.0 / 1000.0);
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
		for (auto& [functionName, timeFrame] : Profile::GetTimeFrames())
		{
			averageTimes.emplace(timeFrame.GetAverageTime(), functionName);
		}

		//Iterator instead of range loop here because we want highest times first.
		for (auto averageTimesIt = averageTimes.rbegin(); averageTimesIt != averageTimes.rend(); averageTimesIt++)
		{
			ImGui::Text(averageTimesIt->second.c_str());
			ImGui::Text(std::to_string(averageTimesIt->first).c_str());
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
		if (Physics::RaycastFromScreen(ray))
		{
			Actor* actor = ray.hitActor;
			if (actor)
			{
				ImGui::Begin("Actor Inspect");
				ImGui::SetWindowPos(ImVec2(Editor::Get().GetViewportMouseX(), Editor::Get().GetViewportMouseY()));

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
