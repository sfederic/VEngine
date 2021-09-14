#pragma once
#include <string>
#include <qdockwidget.h>

struct LogDock : public QDockWidget
{
	struct QTextEdit* logMessageBox;
	struct QPushButton* clearButton;

	LogDock();
	void Print(const std::wstring message);
	void Print(const std::string message);
	void ClearLog();
};
