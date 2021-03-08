#include "ConsoleDock.h"
#include <qlayout.h>
#include <qlineedit.h>
#include <qtextedit.h>

ConsoleDock::ConsoleDock() : QDockWidget(QString("Console"))
{
	auto vbLayout = new QVBoxLayout();

	searchBar = new QLineEdit();
	vbLayout->addWidget(searchBar);
	connect(searchBar, &QLineEdit::returnPressed, this, &ConsoleDock::Search);

	consoleMessageBox = new QTextEdit();
	consoleMessageBox->setReadOnly(true);

	vbLayout->addWidget(consoleMessageBox);

	auto consoleWidget = new QWidget();
	consoleWidget->setLayout(vbLayout);

	setWidget(consoleWidget);
}

//Highlights specified text in search bar
void ConsoleDock::Search()
{
	consoleMessageBox->find(searchBar->text());
}