
#include "EditorMainWindow.h"
#include "imgui_forward_declare.h"
#include "Core/Core.h"
#include "RenderViewWidget.h"
#include "PropertiesDock.h"
#include "AssetDock.h"
#include "WorldDock.h"
#include "ToolbarDock.h"
#include "LogDock.h"
#include "SystemDock.h"
#include "DialogueDock.h"

EditorMainWindow::EditorMainWindow()
{
	renderView = new RenderViewWidget();
	propertiesDock = new PropertiesDock();
	assetDock = new AssetDock();
	logDock = new LogDock();
	worldDock = new WorldDock();
	systemDock = new SystemDock();
	toolbarDock = new ToolbarDock();
	dialogueDock = new DialogueDock();

	//Set dock tabs up top
	setTabPosition(Qt::DockWidgetArea::AllDockWidgetAreas, QTabWidget::TabPosition::North);

	setCentralWidget(renderView);
	addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, propertiesDock);

	addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, assetDock);
	tabifyDockWidget(assetDock, dialogueDock);
	assetDock->raise();

	addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, logDock);
	addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, toolbarDock);

	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, worldDock);
	tabifyDockWidget(worldDock, systemDock);
	worldDock->raise();

	setWindowState(Qt::WindowMaximized);
	show();
}

void EditorMainWindow::Tick()
{
	worldDock->Tick();
}

void EditorMainWindow::closeEvent(QCloseEvent* closeEvent)
{
	Core::mainLoop = false;
}

bool EditorMainWindow::nativeEvent(const QByteArray&, void* message, long*)
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
		systemDock->hide();
		toolbarDock->hide();
		logDock->hide();
		dialogueDock->hide();
	}
	else if (!fullScreenOn)
	{
		propertiesDock->show();
		assetDock->show();
		worldDock->show();
		systemDock->show();
		toolbarDock->show();
		logDock->show();
		dialogueDock->show();
	}
}
