#pragma once
#include "IEditor.h"

struct Win32Editor : IEditor
{
	void Init(int argc, char* argv[]) override;
	void Tick() override;
	void SetMousePos() override;
	void SetupWindow();
	void HandleMessages();
};
