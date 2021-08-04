#include "DebugMenu.h"
#include "UISystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "RenderSystem.h"
#include "Actor.h"
#include "World.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "TransformGizmo.h"
#include "EditorSystem.h"
#include "ActorSystemFactory.h"
#include "Profiler.h"
#include "Raycast.h"
#include "Camera.h"
#include "Properties.h"
#include "WorldEditor.h"
#include <DirectXMath.h>

DebugMenu gDebugMenu;

void DebugMenu::Init()
{
	//IMGUI setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	//Imgui has an .ini file to save previous ui positions and values.
	//Setting this to null removes this initial setup.
	io.IniFilename = nullptr;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init((HWND)gEditorSystem->mainWindow);
	ImGui_ImplDX11_Init(gRenderSystem.device, gRenderSystem.context);
}

void DebugMenu::Tick(World* world, float deltaTime)
{
	//Start ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderNotifications(deltaTime);

	//Keep in mind ImGuizmo has to be called here, it's part of ImGui
	gTransformGizmo.Tick();

	RenderFPSMenu(deltaTime);
	RenderGPUMenu();
	RenderProfileMenu();
	RenderSnappingMenu();
	RenderActorStatsMenu();
	RenderActorSpawnMenu();
	RenderActorInspectMenu();
	RenderActorProps();

	ImGui::EndFrame();
}

void DebugMenu::Cleanup()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugMenu::AddNotification(const wchar_t* note)
{
	notifications.push_back(DebugNotification(note));
}

//Handle notifications (eg. "Shaders recompiled", "ERROR: Not X", etc)
void DebugMenu::RenderNotifications(float deltaTime)
{
	float textOffsetX = 20.f;

	const float notificationLifetime = 3.0f;
	for (int i = 0; i < notifications.size(); i++)
	{
		if (notifications[i].timeOnScreen < notificationLifetime)
		{
			notifications[i].timeOnScreen += deltaTime;

			float notificationOffsetY = 20.f * i;
			gUISystem.d2dRenderTarget->DrawTextA(notifications[i].text.c_str(), notifications[i].text.size(), gUISystem.textFormat,
				{ 0.f, notificationOffsetY, 1000.f, 1000.f }, gUISystem.brushText);
		}
		else
		{
			notifications.erase(notifications.begin() + i);
		}
	}
}

void DebugMenu::RenderFPSMenu(float deltaTime)
{
	if (bFPSMenuOpen)
	{
		ImGui::Begin("FPS");

		ImGui::Text("FPS: %d", gCoreSystem.finalFrameCount);
		ImGui::Text("GPU Render Time: %f", gRenderSystem.renderTime);
		ImGui::Text("Delta Time (ms): %f", deltaTime);
		ImGui::Text("Time Since Startup: %f", gCoreSystem.timeSinceStartup);

		ImGui::End();
	}
}

void DebugMenu::RenderGPUMenu()
{
	if (bGPUMenuOpen)
	{
		ImGui::Begin("GPU Info");

		DXGI_ADAPTER_DESC1 adapterDesc = gRenderSystem.adaptersDesc.front();

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
			for (int i = 1; i < gRenderSystem.adaptersDesc.size(); i++)
			{
				ImGui::Text("Device: %ls", gRenderSystem.adaptersDesc[i].Description);
				ImGui::Text("System Memory: %zu", gRenderSystem.adaptersDesc[i].DedicatedSystemMemory);
				ImGui::Text("Video Memory: %zu", gRenderSystem.adaptersDesc[i].DedicatedVideoMemory);
				ImGui::Text("Shared System Memory: %zu", gRenderSystem.adaptersDesc[i].SharedSystemMemory);
				ImGui::Spacing();
			}
		}

		ImGui::End();
	}
}

void DebugMenu::RenderProfileMenu()
{
	if (bProfileMenuOpen)
	{
		ImGui::Begin("Profiler Time Frames");

		for (auto& timeFrame : gProfiler.timeFrames)
		{
			ImGui::Text(timeFrame.first.c_str());
			double time = timeFrame.second->GetAverageTime();
			ImGui::Text(std::to_string(time).c_str());
		}

		ImGui::End();
	}
}

void DebugMenu::RenderSnappingMenu()
{
	if (bSnapMenuOpen)
	{
		ImGui::Begin("Snapping");

		ImGui::InputFloat("Translation", &gTransformGizmo.translateSnapValues[0]);
		gTransformGizmo.translateSnapValues[1] = gTransformGizmo.translateSnapValues[0];
		gTransformGizmo.translateSnapValues[2] = gTransformGizmo.translateSnapValues[0];

		ImGui::InputFloat("Rotation", &gTransformGizmo.rotationSnapValues[0]);
		gTransformGizmo.rotationSnapValues[1] = gTransformGizmo.rotationSnapValues[0];
		gTransformGizmo.rotationSnapValues[2] = gTransformGizmo.rotationSnapValues[0];

		ImGui::InputFloat("Scale", &gTransformGizmo.scaleSnapValues[0]);
		gTransformGizmo.scaleSnapValues[1] = gTransformGizmo.scaleSnapValues[0];
		gTransformGizmo.scaleSnapValues[2] = gTransformGizmo.scaleSnapValues[0];

		if (gTransformGizmo.currentTransformMode == ImGuizmo::MODE::LOCAL)
		{
			ImGui::Text("LOCAL");
		}
		else if (gTransformGizmo.currentTransformMode == ImGuizmo::MODE::WORLD)
		{
			ImGui::Text("WORLD");
		}

		ImGui::End();
	}
}

void DebugMenu::RenderActorStatsMenu()
{
	if (bActorStatsMenuOpen)
	{
		ImGui::Begin("Actor Stats");

		World* world = GetWorld();
		ImGui::Text("Num actors rendered: %d", world->GetNumOfActorsInWorld());

		ImGui::End();
	}
}

void DebugMenu::RenderActorSpawnMenu()
{
	if (bActorSpawnMenuOpen)
	{
		ImGui::Begin("Active Actor System");
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(200, 50));
		
		ActorSystem* activeAS = ActorSystemFactory::GetCurrentActiveActorSystem();
		if (activeAS)
		{
			ImGui::Text("%s", activeAS->name.c_str());
		}
		else
		{
			ImGui::Text("None");
		}

		ImGui::End();
	}
}

void DebugMenu::RenderActorProps()
{
	PROFILE_START

	if (gWorldEditor.pickedActor == nullptr) return;

	ImGui::Begin("Actor Props");
	//ImGui::SetWindowPos(ImVec2(10, 10));
	//ImGui::SetWindowSize(ImVec2(300, 500));

	auto props = gWorldEditor.pickedActor->GetProps();

	for (auto& actorProperty : props.dataMap)
	{
		auto& typeMap = props.typeMap;
		auto type = typeMap.find(actorProperty.first);

		if (type->second == typeid(bool))
		{
			ImGui::Checkbox(actorProperty.first.c_str(), (bool*)actorProperty.second);
		}
		else if (type->second == typeid(int))
		{
			ImGui::InputInt(actorProperty.first.c_str(), (int*)actorProperty.second);
		}
		else if (type->second == typeid(float))
		{
			ImGui::InputFloat(actorProperty.first.c_str(), (float*)actorProperty.second);
		}
		else if (type->second == typeid(XMFLOAT3))
		{
			DirectX::XMFLOAT3* xmfloat3 = (DirectX::XMFLOAT3*)actorProperty.second;

			//ImGui actually uses the Label as the unique ID to a widget, that's why
			//the strings below are made to be unique.
			//REF: https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-why-is-my-widget-not-reacting-when-i-click-on-it

			float *f3[3] = { &xmfloat3->x, &xmfloat3->y, &xmfloat3->z };

			ImGui::InputFloat3(actorProperty.first.c_str(), *f3);
		}
		else if (type->second == typeid(std::string))
		{
			std::string* str = static_cast<std::string*>(actorProperty.second);
			ImGui::InputText("name", str->data(), str->size());
		}
	}

	ImGui::End();

	PROFILE_END
}

void DebugMenu::RenderActorInspectMenu()
{
	if (bActorInspectMenuOpen)
	{
		ImGui::Begin("Actor Inspect");
		ImGui::SetWindowPos(ImVec2(gUISystem.mousePos.x, gUISystem.mousePos.y));
		ImGui::SetWindowSize(ImVec2(300, 250));

		Ray ray;
		if (RaycastAllFromScreen(ray))
		{
			Actor* actor = ray.hitActor;
			if (actor)
			{
				//TODO: this is just dummy data to see what the menu can do.
				//Come back here when there is more actor specific instance data to show
				//eg. materials, buffers, other weird things
				ImGui::Text("Linked System: %s", actor->linkedActorSystem->name.c_str());
				ImGui::Text("Shader: %s", actor->linkedActorSystem->shaderName.c_str());
				ImGui::Text("Texture: %s", actor->linkedActorSystem->textureName.c_str());
				ImGui::Text("Model: %s", actor->linkedActorSystem->modelName.c_str());
			}
		}

		ImGui::End();
	}
}
