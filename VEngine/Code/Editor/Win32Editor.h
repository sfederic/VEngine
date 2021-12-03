#pragma once
#include "IEditor.h"

struct Win32Editor : IEditor
{
	void Init(int argc, char* argv[]) override;
	void Tick() override;
	void SetMousePos() override;
	virtual void Log(const std::wstring logMessage) override;
	virtual void Log(const std::string logMessage) override;
	virtual void SetActorProps(Actor* actor) override;
	virtual void UpdateWorldList() override;
	virtual void RefreshAssetList() override;
	virtual void ClearProperties() override;
	virtual void SelectActorInWorldList() override;
	virtual void SetPlayButtonText(std::string text) override;
	void SetupWindow();
	void HandleMessages();
};
