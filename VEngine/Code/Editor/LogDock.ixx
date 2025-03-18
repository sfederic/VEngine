#include <qdockwidget.h>

export module Editor.LogDock;

import <string>;

class QTextEdit;
class QPushButton;

export class LogDock : public QDockWidget
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
