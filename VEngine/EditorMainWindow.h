#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EditorMainWindow.h"
#include "ToolbarDock.h"
#include "WorldDock.h"
#include "PropertiesDock.h"
#include "AssetDock.h"
#include "RenderViewWidget.h"

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorMainWindow(QWidget *parent = Q_NULLPTR);

	void closeEvent(QCloseEvent* event) override;

	//NOTE: mouse wheel button and wheel events don't call here. Function in renderviewwidget calls them.
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	void Tick();

	//Docks
	ToolbarDock* toolbarDock;
	WorldDock* worldDock;
	PropertiesDock* propertiesDock;
	AssetDock* assetDock;

	RenderViewWidget* renderViewWidget;

private:
    Ui::EditorMainWindowClass ui;
};

