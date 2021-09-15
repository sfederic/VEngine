#include "LogDock.h"
#include <qtextedit.h>
#include <QVBoxLayout>
#include <QPushButton>

LogDock::LogDock() : QDockWidget("Log")
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

void LogDock::Print(const std::wstring message)
{
	logMessageBox->insertPlainText(QString::fromStdWString(message) + "\n");
	logMessageBox->moveCursor(QTextCursor::MoveOperation::End);
}

void LogDock::Print(const std::string message)
{
	logMessageBox->insertPlainText(QString::fromStdString(message) + "\n");
	logMessageBox->moveCursor(QTextCursor::MoveOperation::End);
}

void LogDock::ClearLog()
{
	logMessageBox->clear();
}
