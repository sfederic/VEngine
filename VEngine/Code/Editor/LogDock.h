#pragma once
#include <string>
#include <qdockwidget.h>

class QTextEdit;
class QPushButton;

struct LogDock : public QDockWidget
{
	QTextEdit* logMessageBox = nullptr;
	QPushButton* clearButton = nullptr;

	LogDock();
	void Print(const std::wstring message);
	void Print(const std::string message);
	void ClearLog();
};
