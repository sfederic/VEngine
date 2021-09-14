#pragma once
#include "IEditor.h"

//TODO: a lot of empty virtual functions. Come back and see whether it's worth making imgui equivalents
//of all the Qt controls in the editor.

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
	virtual void OpenMaterialEditor(const std::string materialFilename) override;
	void SetupWindow();
	void HandleMessages();
};
