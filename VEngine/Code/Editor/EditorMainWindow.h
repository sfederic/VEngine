#pragma once
#include <QtWidgets/QMainWindow>

struct EditorMainWindow : public QMainWindow
{
	struct RenderViewWidget* renderView;
	struct PropertiesDock* propertiesDock;
	struct AssetDock* assetDock;
	struct WorldDock* worldDock;
	struct SystemDock* systemDock;
	struct ToolbarDock* toolbarDock;
	struct LogDock* logDock;
	struct DialogueDock* dialogueDock;

	bool fullScreenOn = false;

	EditorMainWindow();
	void Tick();
	void closeEvent(QCloseEvent* closeEvent) override;
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
	void HideAllDocks();
};
