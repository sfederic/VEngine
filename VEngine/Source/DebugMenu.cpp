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

	//Keep in mind ImGuizmo has to be called here to, it's part of ImGui
	gTransformGizmo.Tick();

	//ImGui profile menu
	if (gInputSystem.GetKeyUpState(Keys::_5))
	{
		bProfileMenuOpen = !bProfileMenuOpen;
	}

	if (bProfileMenuOpen)
	{
		ImGui::Begin("Profiler Time Frames");
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(300, 300));

		for (auto& timeFrame : gProfiler.timeFrames)
		{
			ImGui::Text(timeFrame.first.c_str());
			double time = timeFrame.second->GetAverageTime();
			ImGui::Text(std::to_string(time).c_str());
		}

		ImGui::End();
	}

	//FPS Menu
	if (gInputSystem.GetKeyUpState(Keys::_6))
	{
		bFPSMenuOpen = !bFPSMenuOpen;
	}

	if (bFPSMenuOpen)
	{
		ImGui::Begin("FPS");
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(300, 100));
		
		ImGui::Text("FPS: %d", gCoreSystem.finalFrameCount);
		ImGui::Text("Delta Time (ms): %f", deltaTime);
		ImGui::Text("Time Since Startup: %f", gCoreSystem.timeSinceStartup);

		ImGui::End();
	}

	//GPU Menu
	if (gInputSystem.GetKeyUpState(Keys::_7))
	{
		bGPUMenuOpen = !bGPUMenuOpen;
	}

	if (bGPUMenuOpen)
	{
		ImGui::Begin("GPU Info");
		ImGui::SetWindowPos(ImVec2(10, 10));
		//ImGui::SetWindowSize(ImVec2(400, 400));

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

	//Current selected actor system menu
	ActorSystem* selectedActorSystem = ActorSystemFactory::GetCurrentActiveActorSystem();
	if(selectedActorSystem)
	{
		ImGui::Begin("Selected Actor System");
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(250, 50));
		ImGui::Text(typeid(*selectedActorSystem).name());
		ImGui::End();
	}

	static int menuCursorIndex;
	static int subMenuCursorIndex;

	float menuHeight = (menuItems.size() * 20.f) + 30.f;
	float menuStartPosX = 10.f;
	float menuStartPosY = 10.f;
	float menuWidth = (float)gCoreSystem.windowWidth / 2.f;
	float textOffsetX = 20.f;

	//Handle notifications (eg. "Shaders recompiled", "ERROR: Not X", etc)
	const float notificationLifetime = 3.0f;
	for (int i = 0; i < notifications.size(); i++)
	{
		if (notifications[i].timeOnScreen < notificationLifetime)
		{
			notifications[i].timeOnScreen += deltaTime;
			float notificationOffsetY = 20.f * i;
			gUISystem.d2dRenderTarget->DrawTextA(notifications[i].text, wcslen(notifications[i].text), gUISystem.textFormat,
				{ 0.f, notificationOffsetY, 1000.f, 1000.f }, gUISystem.brushText);
		}
		else
		{
			notifications.erase(notifications.begin() + i);
		}
	}

	//IMGUI Snapping menu
	static bool bIsSnappingMenuOpen;
	if (gInputSystem.GetKeyUpState('4'))
	{
		bIsSnappingMenuOpen = !bIsSnappingMenuOpen;
	}

	if (bIsSnappingMenuOpen)
	{
		ImGui::Begin("Snapping");
		ImGui::SetWindowSize(ImVec2(0, 0));
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

	//Open key for menu
	/*if(gInputSystem.GetKeyUpState(VK_TAB))
	{
		bDebugMenuActive = !bDebugMenuActive;
		bSubMenuOpen = false;
		subMenuCursorIndex = 0;
		menuCursorIndex = 0;
	}*/

	if (gInputSystem.GetKeyUpState(VK_BACK))
	{
		bSubMenuOpen = false;
		subMenuCursorIndex = 0;
	}

	//Main debug menu
	if (bDebugMenuActive && !bSubMenuOpen)
	{
		//Main window
		gUISystem.d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, menuHeight }, gUISystem.brushTransparentMenu);

		float textOffsetY = 0.f;

		if (gInputSystem.GetKeyDownState(VK_DOWN))
		{
			if (menuCursorIndex < (menuItems.size() - 1))
			{
				menuCursorIndex++;
			}
		}
		else if (gInputSystem.GetKeyDownState(VK_UP))
		{
			if (menuCursorIndex > 0)
			{
				menuCursorIndex--;
			}
		}

		if (gInputSystem.GetKeyUpState(VK_RETURN))
		{
			//menuItems[menuCursorIndex].Open();
			//bSubMenuOpen = true;
		}

		//menu items
		for (int i = 0; i < menuItems.size(); i++)
		{
			textOffsetY += 20.f;

			if (menuCursorIndex != i)
			{
				gUISystem.d2dRenderTarget->DrawTextA(menuItems[i].name, wcslen(menuItems[i].name), gUISystem.textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + textOffsetY, menuWidth, textOffsetY }, gUISystem.brushText);
			}
			else
			{
				gUISystem.d2dRenderTarget->DrawTextA(menuItems[i].name, wcslen(menuItems[i].name), gUISystem.textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + textOffsetY, menuWidth, textOffsetY }, gUISystem.brushTransparentMenu);
			}
		}
	}

	//Handle opened debug menu option
	if (bSubMenuOpen)
	{
		if (gInputSystem.GetKeyUpState(VK_RETURN))
		{
			//... On/Off code for various switches
			//gRenderSystem.bQueryGPU = false;
			//gRenderSystem.bQueryGPUInner = false;
		}

		float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f) + 30.f;

		//Draw menu background
		gUISystem.d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, subMenuHeight }, gUISystem.brushTransparentMenu);

		float subMenuTextOffsetY = 0.f;

		//This is terrible, but I can't think of any other way for now (testing)
		switch (menuItems[menuCursorIndex].menuID)
		{
		case EMenuID::ACTORS:
			wchar_t actorCount[64];
			//_snwprintf_s(actorCount, sizeof(actorCount), L"Actor Count: %d", 
				//world->actorSystems.size() * world->actorSystems);
			menuItems[menuCursorIndex].subMenuItems[0] = actorCount;
			break;

		case EMenuID::RENDERING:
			//gRenderSystem.bQueryGPU = true;

			wchar_t renderText[64];
			//_snwprintf_s(renderText, sizeof(renderText), L"D3D11 Timer: %f", gRenderSystem.renderTime);
			menuItems[menuCursorIndex].subMenuItems[0] = renderText;

			wchar_t gpuText[64];
			//_snwprintf_s(gpuText, sizeof(gpuText), L"GPU: %ls", gRenderSystem.adaptersDesc[0].Description);
			menuItems[menuCursorIndex].subMenuItems[1] = gpuText;
			break;

		default:
			//gRenderSystem.bQueryGPU = false;
			break;
		}

		//Submenu items
		/*for (int i = 0; i < menuItems[menuCursorIndex].subMenuItems.size(); i++)
		{
			subMenuTextOffsetY += 20.f;
			float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f);

			gUISystem.d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i],
				wcslen(menuItems[menuCursorIndex].subMenuItems[i]), gUISystem.textFormat, { menuStartPosX + textOffsetX,
				menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuTextOffsetY }, gUISystem.brushText);

			//Was for moving through submenus with arrow keys.
			if (menuCursorIndex != i)
			{
				gUISystem.d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i], 
					wcslen(menuItems[menuCursorIndex].subMenuItems[i]), gUISystem.textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuHeight }, gUISystem.brushText);
			}
			else
			{
				gUISystem.d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i],
					wcslen(menuItems[menuCursorIndex].subMenuItems[i]), gUISystem.textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuHeight }, gUISystem.brushTransparentMenu);
			}
		}*/
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
