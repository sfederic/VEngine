#pragma once

#include "IEditorSystem.h"

//For any 'native' editor (Win32) stuff for builds. 
//Maybe can even get away with using Imgui or D2D as a Qt replacement.
class NativeEditorSystem : public IEditorSystem
{
public:
	virtual void Init(int argc, char** argv);
	virtual void Tick() {}
	virtual void PopulateWorldList() {}
	virtual void ProcessEvents() {}
	virtual void DisplayActorSystemProperties(Actor* actor) {}
	virtual void SetWindowWidthHeight() {}
	virtual void SetDockFocus(EDockFocus focus) {}
	virtual int GetDockFocus() { return 0; }
	virtual void GetMousePos(int* x, int* y) {}
	virtual void ToggleFullscreen() {}
};
