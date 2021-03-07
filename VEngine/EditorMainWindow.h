#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EditorMainWindow.h"

class ConsoleDock;
class ToolbarDock;
class WorldDock;
class PropertiesDock;
class AssetDock;
class ConsoleDock;
class RenderViewWidget;

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorMainWindow(QWidget *parent = Q_NULLPTR);

	void closeEvent(QCloseEvent* event) override;

	//NOTE: mouse wheel button and wheel events don't call here. Function in renderviewwidget calls them.
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	void Tick();
	void Print(const char* string); //Print to consoledock's TextEdit

	//Docks
	//TODO: why aren't these forward declared? fix up
	ToolbarDock* toolbarDock;
	WorldDock* worldDock;
	PropertiesDock* propertiesDock;
	AssetDock* assetDock;
	ConsoleDock* consoleDock;

	RenderViewWidget* renderViewWidget;

private:
    Ui::EditorMainWindowClass ui;
};
