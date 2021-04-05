#include "ToolkitEditorSystem.h"
#include "WorldDock.h"
#include "CoreSystem.h"
#include "RenderViewWidget.h"
#include "PropertiesDock.h"
#include "Actor.h"
#include "Input.h"

QApplication* qApplication;
EditorMainWindow* editorMainWindow;

EDockFocus currentDockFocus;

void ToolkitEditorSystem::Init(int argc, char** argv)
{
	qApplication = new QApplication(argc, argv);
	editorMainWindow = new EditorMainWindow();
	mainWindow = (void*)editorMainWindow->renderViewWidget->winId();
}

void ToolkitEditorSystem::Tick()
{
	editorMainWindow->Tick();
	ToggleFullscreen();
}

void ToolkitEditorSystem::PopulateWorldList()
{
	editorMainWindow->worldDock->PopulateWorldList();
}

void ToolkitEditorSystem::ProcessEvents()
{
	qApplication->processEvents();
}

void ToolkitEditorSystem::DisplayActorSystemProperties(Actor* actor)
{
	editorMainWindow->propertiesDock->DisplayActorSystemProperties(actor);
}

void ToolkitEditorSystem::SetWindowWidthHeight()
{
	gCoreSystem.windowWidth = editorMainWindow->renderViewWidget->size().width();
	gCoreSystem.windowHeight = editorMainWindow->renderViewWidget->size().height();
}

void ToolkitEditorSystem::SetDockFocus(EDockFocus focus)
{
	currentDockFocus = focus;
}

int ToolkitEditorSystem::GetDockFocus()
{
	return (int)editorMainWindow->currentDockFocus;
}

void ToolkitEditorSystem::GetMousePos(int* x, int* y)
{
	QPoint p = editorMainWindow->renderViewWidget->mapFromGlobal(QCursor::pos());
	*x = p.x();
	*y = p.y();
}

void ToolkitEditorSystem::ToggleFullscreen()
{
	static bool fullscreen;

	if (gInputSystem.GetKeyUpState(VK_F11))
	{
		fullscreen = !fullscreen;

		if (fullscreen)
		{
			editorMainWindow->renderViewWidget->setParent(nullptr);
			editorMainWindow->renderViewWidget->setWindowFlags(editorMainWindow->renderViewWidget->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
			editorMainWindow->renderViewWidget->setWindowState(editorMainWindow->renderViewWidget->windowState() | Qt::WindowFullScreen);
			editorMainWindow->renderViewWidget->show();
		}
		else if (!fullscreen)
		{
			editorMainWindow->renderViewWidget->setParent(editorMainWindow);
			editorMainWindow->setCentralWidget(editorMainWindow->renderViewWidget);
		}
	}
}
