#pragma once

#include <qdockwidget.h>

class ToolbarDock : public QDockWidget
{
public:
	ToolbarDock(const char* title);
	void Tick();
	void CreateNewActorSystem();
	void SetActorSystemGenerateArgs();
	void SetPlayMode();
};
