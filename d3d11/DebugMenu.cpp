#include "DebugMenu.h"
#include "UISystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "RenderSystem.h"
#include "Actor.h"
#include "World.h"

DebugMenu debugMenu;

DebugMenu::DebugMenu()
{
	menuItems.push_back(MenuItem(L"Actors", EMenuID::ACTORS));
		menuItems[(int)EMenuID::ACTORS].subMenuItems.push_back(L"Actor Count: ");

	menuItems.push_back(MenuItem(L"Actor Systems", EMenuID::ACTORSYSTEMS));

	menuItems.push_back(MenuItem(L"Rendering", EMenuID::RENDERING));
		menuItems[(int)EMenuID::RENDERING].subMenuItems.push_back(L"D3D11 Timer: ");
		menuItems[(int)EMenuID::RENDERING].subMenuItems.push_back(L"GPU: ");
}

void DebugMenu::Tick(World* world, float deltaTime)
{
	static int menuCursorIndex;
	static int subMenuCursorIndex;

	float menuHeight = (menuItems.size() * 20.f) + 30.f;
	float menuStartPosX = 10.f;
	float menuStartPosY = 10.f;
	float menuWidth = (float)coreSystem.windowWidth / 2.f;
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

	//Open key for menu
	if(inputSystem.GetKeyUpState(VK_TAB))
	{
		bDebugMenuActive = !bDebugMenuActive;
		bSubMenuOpen = false;
		subMenuCursorIndex = 0;
		menuCursorIndex = 0;
	}

	if (inputSystem.GetKeyUpState(VK_BACK))
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

		if (inputSystem.GetKeyDownState(VK_DOWN))
		{
			if (menuCursorIndex < (menuItems.size() - 1))
			{
				menuCursorIndex++;
			}
		}
		else if (inputSystem.GetKeyDownState(VK_UP))
		{
			if (menuCursorIndex > 0)
			{
				menuCursorIndex--;
			}
		}

		if (inputSystem.GetKeyUpState(VK_RETURN))
		{
			//menuItems[menuCursorIndex].Open();
			bSubMenuOpen = true;
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
		if (inputSystem.GetKeyUpState(VK_RETURN))
		{
			//... On/Off code for various switches
			gRenderSystem.bQueryGPU = false;
			gRenderSystem.bQueryGPUInner = false;
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
			//TODO: make a better system that iterates over all actor systems in world
			//_snwprintf_s(actorCount, sizeof(actorCount), L"Actor Count: %d", 
				//world->actorSystems.size() * world->actorSystems);
			menuItems[menuCursorIndex].subMenuItems[0] = actorCount;
			break;

		case EMenuID::RENDERING:
			gRenderSystem.bQueryGPU = true;

			wchar_t renderText[64];
			_snwprintf_s(renderText, sizeof(renderText), L"D3D11 Timer: %f", gRenderSystem.renderTime);
			menuItems[menuCursorIndex].subMenuItems[0] = renderText;

			wchar_t gpuText[64];
			_snwprintf_s(gpuText, sizeof(gpuText), L"GPU: %ls", gRenderSystem.adaptersDesc[0].Description);
			menuItems[menuCursorIndex].subMenuItems[1] = gpuText;
			break;

		default:
			gRenderSystem.bQueryGPU = false;
			break;
		}

		//Submenu items
		for (int i = 0; i < menuItems[menuCursorIndex].subMenuItems.size(); i++)
		{
			subMenuTextOffsetY += 20.f;
			float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f);

			gUISystem.d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i],
				wcslen(menuItems[menuCursorIndex].subMenuItems[i]), gUISystem.textFormat, { menuStartPosX + textOffsetX,
				menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuTextOffsetY }, gUISystem.brushText);

			//Was for moving through submenus with arrow keys.
			/*if (menuCursorIndex != i)
			{
				ui->d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i], 
					wcslen(menuItems[menuCursorIndex].subMenuItems[i]), ui->textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuHeight }, ui->brushText);
			}
			else
			{
				ui->d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i],
					wcslen(menuItems[menuCursorIndex].subMenuItems[i]), ui->textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuHeight }, ui->brushTransparentMenu);
			}*/
		}
	}
}

void DebugMenu::AddNotification(const wchar_t* note)
{
	notifications.push_back(DebugNotification(note));
}
