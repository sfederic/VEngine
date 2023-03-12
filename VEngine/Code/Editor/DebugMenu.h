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

class DebugMenu
{
public:
	std::vector<DebugNotification> debugNotifications;

	bool fpsMenuOpen = false;
	bool profileMenuOpen = false;
	bool gpuMenuOpen = false;
	bool actorInspectMenuOpen = false;
	bool worldStatsMenuOpen = false;
	bool gameInstaceMenuOpen = false;
	bool actorSystemMenuOpen = false;
	bool componentSystemMenuOpen = false;
	bool consoleCommandsMenuOpen = false;
	bool skeletonViewMenuOpen = false;
	bool skeletalAnimationMenuOpen = false;
	bool physicsMenuOpen = false;
	bool coreMenuOpen = false;
	bool particleMenuOpen = false;
	bool worldMenuOpen = false;

	//Whether the Imgui windows have been clicked
	bool hasMouseFocus = false;

	void Init();
	void Tick(float deltaTime);
	void Cleanup();
	void AddNotification(const std::wstring note);

private:
	void RenderNotifications(float deltaTime);

	void RenderFPSMenu(float deltaTime);
	void RenderGPUMenu();
	void RenderProfileMenu();
	void RenderActorInspectMenu();
	void IterateOverProperties(Properties& props);
	void RenderWorldStats();
	void RenderGameInstanceData();
	void RenderActorSystemMenu();
	void RenderComponentSystemMenu();
	void RenderSkeletonViewMenu();
	void RenderSkeletalAnimationMenu();
	void RenderPhysicsMenu();
	void RenderCoreMenu();
	void RenderParticleMenu();
	void RenderConsoleCommandsMenu();
	void RenderWorldMenu();

	//Ctrl+T menu meant to mimic Visual Studio's class find functionality
	void RenderActorSystemSpawnMenu();

	void RenderTexturePlacementMenu();
	void RenderMaterialPlacementMenu();

	bool actorSystemSpawnMenuOpen = false;
};

extern DebugMenu debugMenu;
