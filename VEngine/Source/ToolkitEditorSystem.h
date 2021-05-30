#pragma once

#include "../EditorMainWindow.h"
#include "IEditorSystem.h"

//A huge wrapper around anything 'editor' (Qt) related so the engine can get around #ifdefs
//that handle game deployment and debugging in base windows (win32). 'Toolkit' here refers to Qt.
class ToolkitEditorSystem : public IEditorSystem
{
public:
	virtual void Init(int argc, char** argv);
	virtual void Tick();
	virtual void PopulateWorldList();
	virtual void PopulateActorSystemList();
	virtual void ProcessEvents();
	virtual void DisplayActorSystemProperties(Actor* actor);
	virtual void SetWindowWidthHeight();
	virtual void SetDockFocus(EDockFocus focus);
	virtual int GetDockFocus();
	virtual void GetMousePos(int* x, int* y);
	virtual void ToggleFullscreen();
	virtual void EnableEditorDocks();
	virtual void DisableEditorDocks();
	virtual void Print(const std::string& msg);
};
