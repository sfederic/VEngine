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

	//Keep in mind ImGuizmo has to be called here, it's part of ImGui
	gTransformGizmo.Tick();

	RenderFPSMenu(deltaTime);
	RenderGPUMenu();
	RenderProfileMenu();
	RenderSnappingMenu();

	float textOffsetX = 20.f;

	//Handle notifications (eg. "Shaders recompiled", "ERROR: Not X", etc)
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

void DebugMenu::RenderFPSMenu(float deltaTime)
{
	if (gInputSystem.GetKeyUpState(Keys::_6))
	{
		bFPSMenuOpen = !bFPSMenuOpen;
	}

	if (bFPSMenuOpen)
	{
		ImGui::Begin("FPS");

		ImGui::Text("FPS: %d", gCoreSystem.finalFrameCount);
		ImGui::Text("GPU Render Time: %d", gRenderSystem.renderTime);
		ImGui::Text("Delta Time (ms): %f", deltaTime);
		ImGui::Text("Time Since Startup: %f", gCoreSystem.timeSinceStartup);

		ImGui::End();
	}
}

void DebugMenu::RenderGPUMenu()
{
	if (gInputSystem.GetKeyUpState(Keys::_7))
	{
		bGPUMenuOpen = !bGPUMenuOpen;
	}

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
	if (gInputSystem.GetKeyUpState(Keys::_5))
	{
		bProfileMenuOpen = !bProfileMenuOpen;
	}

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
	if (gInputSystem.GetKeyUpState(Keys::_4))
	{
		bSnapMenuOpen = !bSnapMenuOpen;
	}

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
