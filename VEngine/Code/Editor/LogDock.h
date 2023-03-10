#pragma once

#include <string>
#include <qdockwidget.h>

class QTextEdit;
class QPushButton;

class LogDock : public QDockWidget
{
public:
	LogDock();
	void Print(const std::wstring message);
	void Print(const std::string message);
	void ClearLog();

private:
	QTextEdit* logMessageBox = nullptr;
	QPushButton* clearButton = nullptr;
};
