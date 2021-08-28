#include <Windows.h>
#include "imgui.h"
#include "ImGuizmo.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "DebugMenu.h"
#include "Editor.h"
#include "Render/Renderer.h"
#include "TransformGizmo.h"
#include "Core.h"
#include "Profile.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Components/Component.h"
#include "UI/UISystem.h"
#include "Commands/CommandSystem.h"

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
	ImGui_ImplDX11_Init(renderer.device.Get(), renderer.context.Get());
}

void DebugMenu::Tick(double deltaTime)
{
	//Start ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderNotifications(deltaTime);

	//ImGuizmo has to be called here, it's part of ImGui
	transformGizmo.Tick();

	RenderFPSMenu(deltaTime);
	//RenderGPUMenu();
	RenderProfileMenu();
	RenderSnappingMenu();
	//RenderActorStatsMenu();
	//RenderActorSpawnMenu();
	//RenderActorInspectMenu();
	RenderActorProps();
	RenderCommandsMenu();

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

void DebugMenu::AddNotification(const wchar_t* note)
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
	auto props = worldEditor.pickedActor->GetProps();
	IterateOverProperties(props);

	//Go over component properties
	for (Component* component : worldEditor.pickedActor->components)
	{
		Properties componentProps = component->GetProps();
		IterateOverProperties(componentProps);
	}

	ImGui::End();
}

void DebugMenu::IterateOverProperties(Properties& props)
{
	ImGui::Text(props.title.c_str());

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

			float* f3[3] = { &xmfloat3->x, &xmfloat3->y, &xmfloat3->z };

			ImGui::InputFloat3(actorProperty.first.c_str(), *f3);
		}
		else if (type->second == typeid(std::string))
		{
			std::string* str = static_cast<std::string*>(actorProperty.second);
			ImGui::InputText("name", str->data(), str->size());
		}
	}
}

void DebugMenu::RenderCommandsMenu()
{
	if (!commandsMenuOpen) return;

	ImGui::Begin("Commands");

	if (ImGui::BeginListBox("First"))
	{
		unsigned int nameCount = 0;

		for (ICommand* command : commandSystem.commands)
		{
			std::string name = command->name + std::to_string(nameCount);

			if (ImGui::Selectable(name.c_str()))
			{
				commandSystem.WindToCommand(nameCount);
			}

			nameCount++;
		}
		ImGui::EndListBox();
	}

	if (ImGui::Button("Clear Commands"))
	{
		commandSystem.Reset();
	}

	ImGui::End();
}

//Handle notifications (eg. "Shaders recompiled", "ERROR: Not X", etc)
void DebugMenu::RenderNotifications(double deltaTime)
{
	float textOffsetX = 20.f;

	const float notificationLifetime = 3.0f;
	for (int i = 0; i < debugNotifications.size(); i++)
	{
		if (debugNotifications[i].timeOnScreen < notificationLifetime)
		{
			debugNotifications[i].timeOnScreen += deltaTime;

			float notificationOffsetY = 20.f * i;
			uiSystem.d2dRenderTarget->DrawTextA(debugNotifications[i].text.c_str(), debugNotifications[i].text.size(), uiSystem.textFormat,
				{ 0.f, notificationOffsetY, 1000.f, 1000.f }, uiSystem.brushText);
		}
		else
		{
			debugNotifications.erase(debugNotifications.begin() + i);
		}
	}
}

void DebugMenu::RenderFPSMenu(double deltaTime)
{
	Profile::Start(std::source_location::current());

	if (fpsMenuOpen)
	{
		ImGui::Begin("FPS");

		ImGui::Text("FPS: %d", Core::finalFrameCount);
		//ImGui::Text("GPU Render Time: %f", renderer.renderTime);
		ImGui::Text("Delta Time (ms): %f", deltaTime);
		ImGui::Text("Time Since Startup: %f", Core::timeSinceStartup);

		ImGui::End();
	}

	Profile::End(std::source_location::current());
}

//void DebugMenu::RenderGPUMenu()
//{
//	if (bGPUMenuOpen)
//	{
//		ImGui::Begin("GPU Info");

//		DXGI_ADAPTER_DESC1 adapterDesc = gRenderSystem.adaptersDesc.front();

//		ImGui::Text("Device: %ls", adapterDesc.Description);
//		ImGui::Text("System Memory: %zu", adapterDesc.DedicatedSystemMemory);
//		ImGui::Text("Video Memory: %zu", adapterDesc.DedicatedVideoMemory);
//		ImGui::Text("Shared System Memory: %zu", adapterDesc.SharedSystemMemory);
//		ImGui::Spacing();

//		static bool showAllDevices;
//		if (!showAllDevices && ImGui::Button("Show all Devices"))
//		{
//			showAllDevices = true;
//		}
//		else if (showAllDevices && ImGui::Button("Hide all Devices"))
//		{
//			showAllDevices = false;
//		}

//		if (showAllDevices)
//		{
//			for (int i = 1; i < gRenderSystem.adaptersDesc.size(); i++)
//			{
//				ImGui::Text("Device: %ls", gRenderSystem.adaptersDesc[i].Description);
//				ImGui::Text("System Memory: %zu", gRenderSystem.adaptersDesc[i].DedicatedSystemMemory);
//				ImGui::Text("Video Memory: %zu", gRenderSystem.adaptersDesc[i].DedicatedVideoMemory);
//				ImGui::Text("Shared System Memory: %zu", gRenderSystem.adaptersDesc[i].SharedSystemMemory);
//				ImGui::Spacing();
//			}
//		}

//		ImGui::End();
//	}
//}

void DebugMenu::RenderProfileMenu()
{
	if (profileMenuOpen)
	{
		ImGui::Begin("Profiler Time Frames");

		for (auto& timeFrame : Profile::timeFrames)
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

//void DebugMenu::RenderActorStatsMenu()
//{
//	if (bActorStatsMenuOpen)
//	{
//		ImGui::Begin("Actor Stats");

//		World* world = GetWorld();
//		ImGui::Text("Num actors rendered: %d", world->GetNumOfActorsInWorld());

//		ImGui::End();
//	}
//}

//void DebugMenu::RenderActorSpawnMenu()
//{
//	if (bActorSpawnMenuOpen)
//	{
//		ImGui::Begin("Active Actor System");
//		ImGui::SetWindowPos(ImVec2(10, 10));
//		ImGui::SetWindowSize(ImVec2(200, 50));

//		ActorSystem* activeAS = ActorSystemFactory::GetCurrentActiveActorSystem();
//		if (activeAS)
//		{
//			ImGui::Text("%s", activeAS->name.c_str());
//		}
//		else
//		{
//			ImGui::Text("None");
//		}

//		ImGui::End();
//	}
//}

//void DebugMenu::RenderActorInspectMenu()
//{
//	if (bActorInspectMenuOpen)
//	{
//		ImGui::Begin("Actor Inspect");
//		ImGui::SetWindowPos(ImVec2(gUISystem.mousePos.x, gUISystem.mousePos.y));
//		ImGui::SetWindowSize(ImVec2(300, 250));

//		Ray ray;
//		if (RaycastAllFromScreen(ray))
//		{
//			Actor* actor = ray.hitActor;
//			if (actor)
//			{
//				//TODO: this is just dummy data to see what the menu can do.
//				//Come back here when there is more actor specific instance data to show
//				//eg. materials, buffers, other weird things
//				ImGui::Text("Linked System: %s", actor->linkedActorSystem->name.c_str());
//				ImGui::Text("Shader: %s", actor->linkedActorSystem->shaderName.c_str());
//				ImGui::Text("Texture: %s", actor->linkedActorSystem->textureName.c_str());
//				ImGui::Text("Model: %s", actor->linkedActorSystem->modelName.c_str());
//			}
//		}

//		ImGui::End();
//	}
//}
