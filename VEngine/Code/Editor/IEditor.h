#pragma once
#include <string>

struct Actor;
struct IActorSystem;

//Base class for Toolkit and Native editors
struct IEditor
{
	void* windowHwnd;

	int viewportMouseX;
	int viewportMouseY;

	int viewportWidth;
	int viewportHeight;

	virtual void Init(int argc, char* argv[]) = 0;
	virtual void Tick() = 0;
	virtual void SetMousePos() = 0;
	virtual void Log(const std::wstring logMessage) = 0;
	virtual void Log(const std::string logMessage) = 0;
	virtual void SetActorProps(Actor* actor) = 0;
	virtual void UpdateWorldList() = 0;
	virtual void AddActorToWorldList(Actor* actor) = 0;
	virtual void RemoveActorFromWorldList(Actor* actor) = 0;
	virtual void RefreshAssetList() = 0;
	virtual void ClearProperties() = 0;
	virtual void SelectActorInWorldList() = 0;
	virtual void SetPlayButtonText(std::string text) = 0;
};
