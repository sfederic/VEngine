#include "EditorMainWindow.h"
#include "imgui_forward_declare.h"
#include <QStringList>
#include <QCloseEvent>
#include <qdockwidget.h>
#include <QtWidgets/QMainWindow>
#include "Core.h"
#include "RenderViewWidget.h"
#include "PropertiesDock.h"
#include "AssetDock.h"
#include "WorldDock.h"
#include "ToolbarDock.h"
#include "LogDock.h"

EditorMainWindow::EditorMainWindow()
{
	renderView = new RenderViewWidget();
	propertiesDock = new PropertiesDock();
	assetDock = new AssetDock();
	logDock = new LogDock();
	worldDock = new WorldDock();
	toolbarDock = new ToolbarDock();

	setCentralWidget(renderView);
	addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, propertiesDock);
	addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, assetDock);
	addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, logDock);
	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, worldDock);
	addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, toolbarDock);

	setWindowState(Qt::WindowMaximized);
	setWindowTitle("VEngine | 2.0");
	show();
}

void EditorMainWindow::closeEvent(QCloseEvent* closeEvent)
{
	Core::mainLoop = false;
}

bool EditorMainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	MSG* msg = (MSG*)message;

	if (ImGui_ImplWin32_WndProcHandler(msg->hwnd, msg->message, msg->wParam, msg->lParam))
	{
		return true;
	}

	Core::HandleWin32MessagePump(msg->message, msg->wParam, msg->lParam);

	return false;
}

void EditorMainWindow::HideAllDocks()
{
	fullScreenOn = !fullScreenOn;

	if (fullScreenOn)
	{
		propertiesDock->hide();
		assetDock->hide();
		worldDock->hide();
		toolbarDock->hide();
		logDock->hide();
	}
	else if (!fullScreenOn)
	{
		propertiesDock->show();
		assetDock->show();
		worldDock->show();
		toolbarDock->show();
		logDock->show();
	}
}
