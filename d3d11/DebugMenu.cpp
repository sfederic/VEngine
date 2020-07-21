#include "DebugMenu.h"
#include "UISystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "RenderSystem.h"
#include "Actor.h"
#include "World.h"

DebugMenu debugMenu;

//float menuHeight = (float)windowHeight - 100.f;
int menuCursorIndex = 0;
int subMenuCursorIndex = 0;

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
			uiSystem.d2dRenderTarget->DrawTextA(notifications[i].text, wcslen(notifications[i].text), uiSystem.textFormat,
				{ 0.f, notificationOffsetY, 1000.f, 1000.f }, uiSystem.brushText);
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
		uiSystem.d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, menuHeight }, uiSystem.brushTransparentMenu);

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
				uiSystem.d2dRenderTarget->DrawTextA(menuItems[i].name, wcslen(menuItems[i].name), uiSystem.textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + textOffsetY, menuWidth, textOffsetY }, uiSystem.brushText);
			}
			else
			{
				uiSystem.d2dRenderTarget->DrawTextA(menuItems[i].name, wcslen(menuItems[i].name), uiSystem.textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + textOffsetY, menuWidth, textOffsetY }, uiSystem.brushTransparentMenu);
			}
		}
	}

	//Handle opened debug menu option
	if (bSubMenuOpen)
	{
		if (inputSystem.GetKeyUpState(VK_RETURN))
		{
			//... On/Off code for various switches
			renderSystem.bQueryGPU = false;
			renderSystem.bQueryGPUInner = false;
		}

		float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f) + 30.f;

		//Draw menu background
		uiSystem.d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, subMenuHeight }, uiSystem.brushTransparentMenu);

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
			renderSystem.bQueryGPU = true;

			wchar_t renderText[64];
			_snwprintf_s(renderText, sizeof(renderText), L"D3D11 Timer: %f", renderSystem.renderTime);
			menuItems[menuCursorIndex].subMenuItems[0] = renderText;

			wchar_t gpuText[64];
			_snwprintf_s(gpuText, sizeof(gpuText), L"GPU: %ls", renderSystem.adaptersDesc[0].Description);
			menuItems[menuCursorIndex].subMenuItems[1] = gpuText;
			break;

		default:
			renderSystem.bQueryGPU = false;
			break;
		}

		//Submenu items
		for (int i = 0; i < menuItems[menuCursorIndex].subMenuItems.size(); i++)
		{
			subMenuTextOffsetY += 20.f;
			float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f);

			uiSystem.d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i],
				wcslen(menuItems[menuCursorIndex].subMenuItems[i]), uiSystem.textFormat, { menuStartPosX + textOffsetX,
				menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuTextOffsetY }, uiSystem.brushText);

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
