#include "ToolkitEditorSystem.h"
#include "WorldDock.h"
#include "CoreSystem.h"
#include "RenderViewWidget.h"

QApplication* qApplication;
EditorMainWindow* editorMainWindow;

EDockFocus currentDockFocus;

void ToolkitEditorSystem::Tick()
{
	editorMainWindow->Tick();
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
