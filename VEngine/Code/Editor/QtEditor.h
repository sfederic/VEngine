#pragma once
#include "IEditor.h"

struct QtEditor : IEditor
{
	struct QApplication* app;
	struct EditorMainWindow* mainWindow;

	void Init(int argc, char* argv[]) override;
	void Tick() override;
	void SetMousePos() override;
	virtual void Log(const std::wstring logMessage) override;
	virtual void ActorProps(Actor* actor) override;
	virtual void UpdateWorldList() override;
	void SetEditorFont();
	void EnableDarkMode();
};
