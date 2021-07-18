#pragma once

#include <QtWidgets/QMainWindow>
#include "EditorSystem.h"

class ConsoleDock;
class ToolbarDock;
class WorldDock;
class PropertiesDock;
class AssetDock;
class ConsoleDock;
class RenderViewWidget;

enum class EEditorTheme
{
	Light, 
	Dark
};

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorMainWindow(QWidget *parent = Q_NULLPTR);

	void closeEvent(QCloseEvent* event) override;

	//NOTE: mouse wheel button and wheel events don't call here. Function in renderviewwidget calls them.
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	void Init(int argc);
	void Tick();
	void CloseTab(int tabIndex);

	//Create functions
	void CreateConsoleDock();

	//Close functions
	void CloseConsoleDock();

	//Main Qt Application
	QApplication* gQApplication;

	//Docks
	ToolbarDock* toolbarDock;
	WorldDock* worldDock;
	PropertiesDock* propertiesDock;
	AssetDock* assetDock;
	ConsoleDock* consoleDock;

	RenderViewWidget* renderViewWidget;

	EDockFocus currentDockFocus;

private:

	//Tabs
	QTabWidget* editorTabWidget;
	QMainWindow* worldEditorWindow;

	EEditorTheme editorTheme = EEditorTheme::Dark;
};
