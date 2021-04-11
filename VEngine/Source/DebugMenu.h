#pragma once

#include <vector>

enum class EMenuID
{
	ACTORS,
	ACTORSYSTEMS,
	RENDERING
};

struct MenuItem
{
	MenuItem(const wchar_t* initName, EMenuID id)
	{
		wcsncpy_s(name, initName, 32);
		menuID = id;
	}

	std::vector<const wchar_t*> subMenuItems;
	wchar_t name[32];
	EMenuID menuID;
};

struct DebugNotification
{
	DebugNotification(const wchar_t* initText)
	{
		wcsncpy_s(text, initText, 256);
		timeOnScreen = 0.f;
	}

	wchar_t text[256];
	float timeOnScreen;
};

//A stats UI system for showing debug-level information. Encapsulates Immediate Mode GUI as well.
class DebugMenu
{
public:
	void Init();
	void Tick(class World* world, float deltaTime);
	void Cleanup();
	void AddNotification(const wchar_t* note);

	std::vector<MenuItem> menuItems;
	std::vector<DebugNotification> notifications;
	bool bDebugMenuActive = false;
	bool bSubMenuOpen = false;
};

extern DebugMenu gDebugMenu;