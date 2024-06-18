#pragma once

#include "IEditor.h"

class Win32Editor : public IEditor
{
public:
	void Init(int argc, char* argv[]) override;
	void Tick() override;
	void SetMousePos() override;
	virtual void Log(const std::wstring logMessage) override;
	virtual void Log(const std::string logMessage) override;
	virtual void SetActorProps(Actor* actor) override;
	virtual void UpdateWorldList() override;
	virtual void UpdateSystemsList() override {}
	virtual void AddActorToWorldList(Actor* actor) override;
	virtual void RemoveActorFromWorldList() override;
	virtual void RefreshAssetList() override;
	virtual void ClearProperties() override;
	virtual void SelectActorInWorldList() override;
	virtual void SetPlayButtonText() override;
	void SetupWindow();
	void HandleMessages();
};
