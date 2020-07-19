#include "DebugMenu.h"
#include "UISystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "RenderSystem.h"
#include "Actor.h"
#include "World.h"

float menuStartPosX = 10.f;
float menuStartPosY = 10.f;
float menuWidth = (float)coreSystem.windowWidth / 2.f;
float textOffsetX = 20.f;

//float menuHeight = (float)windowHeight - 100.f;
int menuCursorIndex = 0;
int subMenuCursorIndex = 0;

int Print(const char* format, va_list args)
{
	char buff[1024];
	int charCount = vsnprintf(buff, sizeof(buff), format, args);
	OutputDebugString(buff);
	return charCount;
}

int DebugPrint(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	int charsWritten = Print(format, argList);
	return charsWritten;
}

DebugMenu::DebugMenu()
{
	menuItems.push_back(MenuItem(L"Actors", EMenuID::ACTORS));
		menuItems[(int)EMenuID::ACTORS].subMenuItems.push_back(L"Actor Count: ");

	menuItems.push_back(MenuItem(L"Actor Systems", EMenuID::ACTORSYSTEMS));

	menuItems.push_back(MenuItem(L"Rendering", EMenuID::RENDERING));
		menuItems[(int)EMenuID::RENDERING].subMenuItems.push_back(L"D3D11 Timer: ");
		menuItems[(int)EMenuID::RENDERING].subMenuItems.push_back(L"GPU: ");
}

void DebugMenu::Tick(UISystem* ui, RenderSystem* dx, World* world, float deltaTime)
{
	//Handle notifications (eg. "Shaders recompiled", "ERROR: Not X", etc)
	const float notificationLifetime = 3.0f;
	for (int i = 0; i < notifications.size(); i++)
	{
		if (notifications[i].timeOnScreen < notificationLifetime)
		{
			notifications[i].timeOnScreen += deltaTime;
			float notificationOffsetY = 20.f * i;
			ui->d2dRenderTarget->DrawTextA(notifications[i].text, wcslen(notifications[i].text), ui->textFormat,
				{ 0.f, notificationOffsetY, 1000.f, 1000.f }, ui->brushText);
		}
		else
		{
			notifications.erase(notifications.begin() + i);
		}
	}

	//Open key for menu
	if(GetKeyUpState(VK_TAB))
	{
		bDebugMenuActive = !bDebugMenuActive;
		bSubMenuOpen = false;
		subMenuCursorIndex = 0;
	}
	if (GetKeyUpState(VK_BACK))
	{
		bSubMenuOpen = false;
		subMenuCursorIndex = 0;
	}

	//Main debug menu
	if (bDebugMenuActive && !bSubMenuOpen)
	{
		float menuHeight = (menuItems.size() * 20.f) + 30.f;

		//Main window
		ui->d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, menuHeight }, ui->brushTransparentMenu);

		float textOffsetY = 0.f;

		if (GetKeyDownState(VK_DOWN))
		{
			if (menuCursorIndex < (menuItems.size() - 1))
			{
				menuCursorIndex++;
			}
		}
		else if (GetKeyDownState(VK_UP))
		{
			if (menuCursorIndex > 0)
			{
				menuCursorIndex--;
			}
		}

		if (GetKeyUpState(VK_RETURN))
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
				ui->d2dRenderTarget->DrawTextA(menuItems[i].name, wcslen(menuItems[i].name), ui->textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + textOffsetY, menuWidth, textOffsetY }, ui->brushText);
			}
			else
			{
				ui->d2dRenderTarget->DrawTextA(menuItems[i].name, wcslen(menuItems[i].name), ui->textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + textOffsetY, menuWidth, textOffsetY }, ui->brushTransparentMenu);
			}
		}
	}

	//Handle opened debug menu option
	if (bSubMenuOpen)
	{
		if (GetKeyUpState(VK_RETURN))
		{
			//... On/Off code for various switches
			dx->bQueryGPU = false;
			dx->bQueryGPUInner = false;
		}

		float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f) + 30.f;

		//Draw menu background
		ui->d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, subMenuHeight }, ui->brushTransparentMenu);

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
			dx->bQueryGPU = true;

			wchar_t renderText[64];
			_snwprintf_s(renderText, sizeof(renderText), L"D3D11 Timer: %f", dx->renderTime);
			menuItems[menuCursorIndex].subMenuItems[0] = renderText;

			wchar_t gpuText[64];
			_snwprintf_s(gpuText, sizeof(gpuText), L"GPU: %ls", dx->adaptersDesc[0].Description);
			menuItems[menuCursorIndex].subMenuItems[1] = gpuText;
			break;

		default:
			dx->bQueryGPU = false;
			break;
		}

		//Submenu items
		for (int i = 0; i < menuItems[menuCursorIndex].subMenuItems.size(); i++)
		{
			subMenuTextOffsetY += 20.f;
			float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f);

			ui->d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i],
				wcslen(menuItems[menuCursorIndex].subMenuItems[i]), ui->textFormat, { menuStartPosX + textOffsetX,
				menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuTextOffsetY }, ui->brushText);

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
