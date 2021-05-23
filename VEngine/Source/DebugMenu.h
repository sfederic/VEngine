#pragma once

#include <vector>
#include <string>

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
	DebugNotification(const wchar_t* note)
	{
		text = note;
		timeOnScreen = 0.f;
	}

	std::wstring text;
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
	void RenderFPSMenu(float deltaTime);
	void RenderGPUMenu();
	void RenderProfileMenu();
	void RenderSnappingMenu();

	std::vector<MenuItem> menuItems;
	std::vector<DebugNotification> notifications;

	bool bProfileMenuOpen = false;
	bool bFPSMenuOpen = false;
	bool bGPUMenuOpen = false;
	bool bSnapMenuOpen = false;
};

extern DebugMenu gDebugMenu;