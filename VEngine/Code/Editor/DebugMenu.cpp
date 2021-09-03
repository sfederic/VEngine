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
#include "Actors/IActorSystem.h"
#include "Components/Component.h"
#include "UI/UISystem.h"
#include "Commands/CommandSystem.h"
#include "Commands/ICommand.h"
#include "Physics/Raycast.h"
#include "Input.h"

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
	RenderGPUMenu();
	RenderProfileMenu();
	RenderSnappingMenu();
	RenderActorProps();
	RenderCommandsMenu();
	RenderActorInspectMenu();

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
		else if (props.CheckType<std::string>(name))
		{
			std::string* str = props.GetData<std::string>(name);
			ImGui::InputText(name.c_str(), str->data(), str->size());
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

//Stole this from the Fledge Engine https://www.youtube.com/watch?v=WjPiJn9dkxs
//Works by hovering a menu over the current mouse over'd actor.
void DebugMenu::RenderActorInspectMenu()
{
	if (actorInspectMenuOpen)
	{
		Ray ray;
		if (RaycastAllFromScreen(ray))
		{
			Actor* actor = ray.hitActor;
			if (actor)
			{
				ImGui::Begin("Actor Inspect");
				ImGui::SetWindowSize(ImVec2(300, 300));
				ImGui::SetWindowPos(ImVec2(editor->viewportMouseX, editor->viewportMouseY));
				ImGui::End();
			}
		}
	}
}
