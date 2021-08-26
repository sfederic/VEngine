#pragma once
#include <qdockwidget.h>

class QPushButton;

struct ToolbarDock : public QDockWidget
{
	QPushButton* playButton;
	QPushButton* pauseButton;

	ToolbarDock();
	void StartPlay();
};
