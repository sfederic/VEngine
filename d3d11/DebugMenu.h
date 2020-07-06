#pragma once

#include <vector>
#include "UIContext.h"

enum class EMenuID
{
	ACTORS,
	ACTORSYSTEMS
};

struct MenuItem
{
	MenuItem(const wchar_t* initName, EMenuID id)
	{
		wcsncpy_s(name, initName, 32);
		menuID = id;
	}

	std::vector<MenuItem> subMenuItems;
	wchar_t name[32];
	EMenuID menuID;
};

class DebugMenu
{
public:
	DebugMenu();

	void Tick(class UIContext* ui);

	std::vector<MenuItem> menuItems;
	bool bDebugMenuActive = false;
	bool bSubMenuOpen = false;
};

