#include "LogDock.h"
#include <qtextedit.h>
#include <QVBoxLayout>
#include <QPushButton>

LogDock::LogDock() : QDockWidget("Console")
{
	auto vbLayout = new QVBoxLayout();

	logMessageBox = new QTextEdit();
	logMessageBox->setReadOnly(true);

	vbLayout->addWidget(logMessageBox);

	clearButton = new QPushButton("Clear");
	vbLayout->addWidget(clearButton);
	connect(clearButton, &QPushButton::pressed, this, &LogDock::ClearLog);

	auto logWidget = new QWidget();
	logWidget->setLayout(vbLayout);

	setWidget(logWidget);
}

void LogDock::ClearLog()
{
	logMessageBox->clear();
}
