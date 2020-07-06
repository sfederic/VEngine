#include "DebugMenu.h"
#include "UIContext.h"
#include "Win32Util.h"
#include "Input.h"

float menuStartPosX = 10.f;
float menuStartPosY = 10.f;
float menuWidth = (float)windowWidth / 2.f;
float textOffsetX = 20.f;

//float menuHeight = (float)windowHeight - 100.f;
int menuCursorIndex = 0;

DebugMenu::DebugMenu()
{
	menuItems.push_back(MenuItem(L"Actors", EMenuID::ACTORS));
		menuItems[0].subMenuItems.push_back(MenuItem(L"Actor Positions", EMenuID::ACTORS));
		menuItems[0].subMenuItems.push_back(MenuItem(L"Actor Memeory", EMenuID::ACTORS));

	menuItems.push_back(MenuItem(L"Actor Systems", EMenuID::ACTORSYSTEMS));
	menuItems.push_back(MenuItem(L"Rendering", EMenuID::ACTORSYSTEMS));
	menuItems.push_back(MenuItem(L"AI", EMenuID::ACTORSYSTEMS));
}

void DebugMenu::Tick(UIContext* ui)
{
	//Open key
	if(GetKeyUpState(VK_TAB))
	{
		bDebugMenuActive = !bDebugMenuActive;
		bSubMenuOpen = false;
	}

	//Main debug menu
	if (bDebugMenuActive && !bSubMenuOpen)
	{
		float menuHeight = (menuItems.size() * 20.f) + 50.f;

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
					menuStartPosY + textOffsetY, menuWidth, menuHeight }, ui->brushText);
			}
			else
			{
				ui->d2dRenderTarget->DrawTextA(menuItems[i].name, wcslen(menuItems[i].name), ui->textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + textOffsetY, menuWidth, menuHeight }, ui->brushTransparentMenu);
			}
		}
	}

	//Handle opened debug menu option
	if (bSubMenuOpen)
	{
		//MenuItem*/ item = (MenuItem*)&menuItems[menuCursorIndex];
		float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f) + 50.f;

		for (int i = 0; i < menuItems[menuCursorIndex].subMenuItems.size(); i++)
		{
			//Draw menu background
			ui->d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, subMenuHeight }, ui->brushTransparentMenu);
		}

		float subMenuTextOffsetY = 0.f;


		//Submenu items
		for (int i = 0; i < menuItems[menuCursorIndex].subMenuItems.size(); i++)
		{
			subMenuTextOffsetY += 20.f;
			float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f) + 50.f;

			if (menuCursorIndex != i)
			{
				ui->d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i].name, 
					wcslen(menuItems[menuCursorIndex].subMenuItems[i].name), ui->textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuHeight }, ui->brushText);
			}
			else
			{
				ui->d2dRenderTarget->DrawTextA(menuItems[menuCursorIndex].subMenuItems[i].name,
					wcslen(menuItems[menuCursorIndex].subMenuItems[i].name), ui->textFormat, { menuStartPosX + textOffsetX,
					menuStartPosY + subMenuTextOffsetY, menuWidth, subMenuHeight }, ui->brushTransparentMenu);
			}
		}
	}
}

