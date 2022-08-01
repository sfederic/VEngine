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
private:
	std::vector<DebugNotification> permanentNotifications;

public:
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
	bool memoriesMenuOpen = false;
	bool consoleCommandsMenuOpen = false;
	bool skeletonViewMenuOpen = false;
	bool physicsMenuOpen = false;
	bool coreMenuOpen = false;
	bool questMenuOpen = false;
	bool particleMenuOpen = false;
	bool cutsceneSequencerOpen = false;

	//Whether the Imgui windows have been clicked
	bool hasMouseFocus = false;

	void Init();
	void Tick(float deltaTime);
	void Cleanup();
	void AddNotification(const std::wstring note);

private:
	void RenderNotifications(float deltaTime);

	//Permanent notifications are based on states of other systems. They aren't on the screen for a select time.
	void SetupPermanentNotifications();
	void RenderPermanentNotifications();
	void ResetPermanentNotifications();

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
	void RenderMemoriesMenu();
	void RenderSkeletonViewMenu();
	void RenderPhysicsMenu();
	void RenderCoreMenu();
	void RenderQuestMenu();
	void RenderParticleMenu();
};

extern DebugMenu debugMenu;
