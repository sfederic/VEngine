#pragma once
#include <vector>
#include <string>

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

struct DebugMenu
{
	std::vector<DebugNotification> debugNotifications;

	bool fpsMenuOpen = false;
	bool snapMenuOpen = false;
	bool profileMenuOpen = false;

	void Init();
	void Tick(double deltaTime);
	void Cleanup();
	void AddNotification(const wchar_t* note);
	void RenderNotifications(double deltaTime);
	void RenderFPSMenu(double deltaTime);
	void RenderProfileMenu();
	void RenderSnappingMenu();
};

extern DebugMenu debugMenu;
