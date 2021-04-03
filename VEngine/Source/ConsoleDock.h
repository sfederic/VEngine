#pragma once

#include <qdockwidget.h>

class QTextEdit;
class QLineEdit;
class QPushButton;

//Console for printing debug information in-editor
class ConsoleDock : public QDockWidget
{
public:
	ConsoleDock();
	void Search();
	void ClearConsole();

	QLineEdit* searchBar;
	QTextEdit* consoleMessageBox;
	QPushButton* clearButton;
};
