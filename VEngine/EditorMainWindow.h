#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EditorMainWindow.h"
#include "RenderViewWidget.h"
#include "PropertiesWidget.h"
#include "CoreSystem.h"

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorMainWindow(QWidget *parent = Q_NULLPTR);
    MainWidget mainWidget;
    PropertiesWidget* propWidget;

	void closeEvent(QCloseEvent* event) override
	{
		gCoreSystem.bMainLoop = false;
	}

	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	void AssetItemClicked(class QListWidgetItem* listWidgetItem);

private:
    Ui::EditorMainWindowClass ui;
};

