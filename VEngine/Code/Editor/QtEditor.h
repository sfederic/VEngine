#pragma once
#include "IEditor.h"

struct QtEditor : IEditor
{
	struct QApplication* app;
	struct EditorMainWindow* mainWindow;

	void Init(int argc, char* argv[]) override;
	void Tick() override;
	void SetMousePos() override;
	void SetEditorFont();
	void EnableDarkMode();
};
