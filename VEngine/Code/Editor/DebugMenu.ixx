export module Editor.DebugMenu;

import <vector>;
import <string>;
import Editor.DebugNotification;

struct Properties;
class UVPaintWidget;

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
	bool vertexPaintMenuOpen = false;
	bool uvMenuOpen = false;
	bool uvPaintMenuOpen = false;
	bool sequencerOpen = false;
	bool widgetDetailsMenuOpen = false;
	bool localeMenuOpen = false;

	//Whether the Imgui windows have been clicked
	bool hasMouseFocus = false;

	void Init();
	void Tick(float deltaTime);
	void Cleanup();
	void ClearAllStaticNotifications();
	void AddNotification(const std::wstring note);
	void AddStaticNotification(std::wstring note);

	void CreateUVPaintWidget();
	void SetUVPaintMenuTextureFilename();

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
	void RenderCoreMenu();
	void RenderParticleMenu();
	void RenderConsoleCommandsMenu();
	void RenderWorldMenu();
	void RenderVertexPaintMenu();
	void RenderUVMenu();
	void RenderUVPaintMenu();
	void RenderWidgetDetailsMenu();
	void RenderLocaleMenu();

	void RenderTexturePlacementMenu();
	void RenderMaterialPlacementMenu();

	std::string uvPaintMenuTextureFilename;
	UVPaintWidget* uvPaintWidget = nullptr;
};

export DebugMenu debugMenu;
