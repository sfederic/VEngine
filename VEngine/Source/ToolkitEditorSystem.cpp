#include "ToolkitEditorSystem.h"
#include "WorldDock.h"
#include "CoreSystem.h"
#include "RenderViewWidget.h"
#include "PropertiesDock.h"
#include "Actor.h"
#include "Input.h"
#include "ConsoleDock.h"
#include "AssetDock.h"
#include "ToolbarDock.h"
#include <QTextEdit>
#include <qapplication.h>

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

void ToolkitEditorSystem::PopulateActorSystemList()
{
	editorMainWindow->worldDock->PopulateActorSystemList();
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
	return (int)currentDockFocus;
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

	if (gInputSystem.GetKeyUpState(Keys::F11))
	{
		fullscreen = !fullscreen;

		if (fullscreen)
		{
			editorMainWindow->propertiesDock->hide();
			editorMainWindow->assetDock->hide();
			editorMainWindow->consoleDock->hide();
			editorMainWindow->toolbarDock->hide();
			editorMainWindow->worldDock->hide();
		}
		else if (!fullscreen)
		{
			editorMainWindow->propertiesDock->show();
			editorMainWindow->assetDock->show();
			editorMainWindow->consoleDock->show();
			editorMainWindow->toolbarDock->show();
			editorMainWindow->worldDock->show();
		}
	}
}

//With both of these functions, want to keep the console dock enabled coz debugging is sick
void ToolkitEditorSystem::EnableEditorDocks()
{
	editorMainWindow->assetDock->setEnabled(true);
	editorMainWindow->propertiesDock->setEnabled(true);
	editorMainWindow->worldDock->setEnabled(true);
}

void ToolkitEditorSystem::DisableEditorDocks()
{
	editorMainWindow->assetDock->setEnabled(false);
	editorMainWindow->propertiesDock->setEnabled(false);
	editorMainWindow->worldDock->setEnabled(false);
}

void ToolkitEditorSystem::Print(const std::string& msg)
{
	editorMainWindow->consoleDock->consoleMessageBox->insertPlainText(QString(msg.c_str()));
	editorMainWindow->consoleDock->consoleMessageBox->moveCursor(QTextCursor::MoveOperation::End);
}
