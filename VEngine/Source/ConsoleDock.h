#pragma once

#include <qdockwidget.h>

class QTextEdit;
class QLineEdit;

//Console for debug information in-editor
class ConsoleDock : public QDockWidget
{
public:
	ConsoleDock();
	void Search();

	QLineEdit* searchBar;
	QTextEdit* consoleMessageBox;
};

