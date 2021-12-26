#pragma once
#include <vector>
#include <string>

struct Properties;

struct DebugNotification
{
	DebugNotification(const std::wstring note)
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
	bool propsMenuOpen = false;
	bool commandsMenuOpen = false;
	bool gpuMenuOpen = false;
	bool actorInspectMenuOpen = false;
	bool worldStatsMenuOpen = false;
	bool gameInstaceMenuOpen = false;
	bool memoryMenuOpen = false;
	bool actorSystemMenuOpen = false;
	bool componentSystemMenuOpen = false;
	bool intuitionsMenuOpen = false;
	bool consoleCommandsMenuOpen = false;
	bool skeletonViewMenuOpen = false;
	bool physicsMenuOpen = false;
	bool coreMenuOpen = false;

	void Init();
	void Tick(float deltaTime);
	void Cleanup();
	void AddNotification(const std::wstring note);

private:
	void RenderNotifications(float deltaTime);
	void RenderFPSMenu(float deltaTime);
	void RenderGPUMenu();
	void RenderProfileMenu();
	void RenderSnappingMenu();
	void RenderActorInspectMenu();
	void RenderActorProps();
	void IterateOverProperties(Properties& props);
	void RenderCommandsMenu();
	void RenderWorldStats();
	void RenderGameInstanceData();
	void RenderMemoryMenu();
	void RenderActorSystemMenu();
	void RenderComponentSystemMenu();
	void RenderIntuitionsMenu();
	void RenderConsoleCommandsMenu();
	void RenderSkeletonViewMenu();
	void RenderPhysicsMenu();
	void RenderCoreMenu();
};

extern DebugMenu debugMenu;
