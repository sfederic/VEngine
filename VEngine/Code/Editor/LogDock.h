#pragma once
#include <qdockwidget.h>

struct LogDock : public QDockWidget
{
	struct QTextEdit* logMessageBox;
	struct QPushButton* clearButton;

	LogDock();
	void ClearLog();
};
