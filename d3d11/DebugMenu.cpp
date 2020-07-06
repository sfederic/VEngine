#include "DebugMenu.h"
#include "UIContext.h"
#include "Win32Util.h"
#include "Input.h"
#include "DXUtil.h"

float menuStartPosX = 10.f;
float menuStartPosY = 10.f;
float menuWidth = (float)windowWidth / 2.f;
float textOffsetX = 20.f;

//float menuHeight = (float)windowHeight - 100.f;
int menuCursorIndex = 0;

DebugMenu::DebugMenu()
{
	menuItems.push_back(MenuItem(L"Actors", EMenuID::ACTORS));
		menuItems[0].subMenuItems.push_back(L"Actor Values");
		menuItems[0].subMenuItems.push_back(L"Actor Positions");
		menuItems[0].subMenuItems.push_back(L"Actor Memeory dump");

	menuItems.push_back(MenuItem(L"Actor Systems", EMenuID::ACTORSYSTEMS));

	menuItems.push_back(MenuItem(L"Rendering", EMenuID::RENDERING));
		menuItems[(int)EMenuID::RENDERING].subMenuItems.push_back(L"D3D11 Timer: ");
}

void DebugMenu::Tick(UIContext* ui, DXUtil* dx)
{
	//Open key
	if(GetKeyUpState(VK_TAB))
	{
		bDebugMenuActive = !bDebugMenuActive;
		bSubMenuOpen = false;
	}
	if (GetKeyUpState(VK_BACK))
	{
		bSubMenuOpen = false;
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
		float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f) + 20.f;

		//Draw menu background
		ui->d2dRenderTarget->FillRectangle({ menuStartPosX, menuStartPosY, menuWidth, subMenuHeight }, ui->brushTransparentMenu);

		float subMenuTextOffsetY = 0.f;

		switch (menuItems[menuCursorIndex].menuID)
		{
		case EMenuID::RENDERING:
			wchar_t d3dTimer[64];
			_snwprintf_s(d3dTimer, sizeof(d3dTimer), L"D3D11 Timer: %f", dx->renderTime);
			menuItems[menuCursorIndex].subMenuItems[0] = d3dTimer;
		}

		//Submenu items
		for (int i = 0; i < menuItems[menuCursorIndex].subMenuItems.size(); i++)
		{
			subMenuTextOffsetY += 20.f;
			float subMenuHeight = (menuItems[menuCursorIndex].subMenuItems.size() * 20.f);

			if (menuCursorIndex != i)
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
			}
		}
	}
}

