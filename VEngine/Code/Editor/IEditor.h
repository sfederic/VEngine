#pragma once
#include <string>

class Actor;
class IActorSystem;

//Base class for Toolkit and Native editors
struct IEditor
{
	void* windowHwnd = nullptr;

	int viewportMouseX = 0;
	int viewportMouseY = 0;

	int viewportWidth = 0;
	int viewportHeight = 0;

	int centerOffsetX = 0;
	int centerOffsetY = 0;

	virtual void Init(int argc, char* argv[]) = 0;
	virtual void Tick() = 0;
	virtual void SetMousePos() = 0;
	virtual void SetMousePosFPSGameplay() {}
	virtual void Log(const std::wstring logMessage) = 0;
	virtual void Log(const std::string logMessage) = 0;
	virtual void SetActorProps(Actor* actor) = 0;
	virtual void UpdateWorldList() = 0;
	virtual void AddActorToWorldList(Actor* actor) = 0;
	virtual void RemoveActorFromWorldList() = 0;
	virtual void RefreshAssetList() = 0;
	virtual void ClearProperties() = 0;
	virtual void SelectActorInWorldList() = 0;
	virtual void SetPlayButtonText() = 0;
	virtual void SetCurrentTransformMode(const std::string trasnformMode) {}
	virtual void SetEditorTitle(const std::string title) {}
	virtual void ResetPropertyWidgetValues() {}
};
