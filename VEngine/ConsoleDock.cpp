#include "ConsoleDock.h"
#include <qlayout.h>
#include <qlineedit.h>
#include <qtextedit.h>

ConsoleDock::ConsoleDock() : QDockWidget(QString("Console"))
{
	auto vbLayout = new QVBoxLayout();

	auto consoleSearchBar = new QLineEdit();
	vbLayout->addWidget(consoleSearchBar);

	consoleMessageBox = new QTextEdit();
	consoleMessageBox->setReadOnly(true);
	vbLayout->addWidget(consoleMessageBox);

	auto consoleWidget = new QWidget();
	consoleWidget->setLayout(vbLayout);

	setWidget(consoleWidget);
}

//Simply print text argument to console 
void ConsoleDock::Print(const char* string)
{
	consoleMessageBox->insertPlainText(QString(string));
	consoleMessageBox->insertPlainText("\n");
}
