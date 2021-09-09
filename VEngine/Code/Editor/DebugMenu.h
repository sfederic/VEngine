#pragma once
#include <vector>
#include <string>

struct Properties;

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

//TODO: ShowWorldVertexCount() function

struct DebugMenu
{
	std::vector<DebugNotification> debugNotifications;

	bool fpsMenuOpen = false;
	bool snapMenuOpen = false;
	bool profileMenuOpen = false;
	bool propsMenuOpen = false;
	bool commandsMenuOpen = false;
	bool gpuMenuOpen = false;
	bool actorInspectMenuOpen = false;

	void Init();
	void Tick(double deltaTime);
	void Cleanup();
	void AddNotification(const wchar_t* note);
	void RenderNotifications(double deltaTime);
	void RenderFPSMenu(double deltaTime);
	void RenderGPUMenu();
	void RenderProfileMenu();
	void RenderSnappingMenu();
	void RenderActorInspectMenu();
	void RenderActorProps();
	void IterateOverProperties(Properties& props);
	void RenderCommandsMenu();
};

extern DebugMenu debugMenu;
