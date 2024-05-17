#pragma once

#include <QtWidgets/QMainWindow>

class EditorMainWindow : public QMainWindow
{
public:
	class RenderViewWidget* renderView = nullptr;
	class PropertiesDock* propertiesDock = nullptr;
	class AssetDock* assetDock = nullptr;
	class WorldDock* worldDock = nullptr;
	class SystemDock* systemDock = nullptr;
	class ToolbarDock* toolbarDock = nullptr;
	class LogDock* logDock = nullptr;
	class DialogueDock* dialogueDock = nullptr;

	bool fullScreenOn = false;

	EditorMainWindow();
	void Tick();
	void closeEvent(QCloseEvent* closeEvent) override;
	bool nativeEvent(const QByteArray&, void* message, long*) override;
	void HideAllDocks();
};
