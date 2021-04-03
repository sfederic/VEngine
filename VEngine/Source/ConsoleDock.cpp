#include "ConsoleDock.h"
#include <qlayout.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qpushbutton.h>

ConsoleDock::ConsoleDock() : QDockWidget(QString("Console"))
{
	auto vbLayout = new QVBoxLayout();

	auto hbLayout = new QHBoxLayout();

	searchBar = new QLineEdit();
	hbLayout->addWidget(searchBar);
	connect(searchBar, &QLineEdit::returnPressed, this, &ConsoleDock::Search);

	clearButton = new QPushButton("Clear");
	hbLayout->addWidget(clearButton);
	connect(clearButton, &QPushButton::pressed, this, &ConsoleDock::ClearConsole);

	consoleMessageBox = new QTextEdit();
	consoleMessageBox->setReadOnly(true);

	vbLayout->addLayout(hbLayout);
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

void ConsoleDock::ClearConsole()
{
	consoleMessageBox->clear();
}
