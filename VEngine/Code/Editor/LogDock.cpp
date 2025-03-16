
#include "LogDock.h"
#include <qtextedit.h>
#include <QVBoxLayout>
#include <QPushButton>

const int MAX_LOG_CHAR_LENGTH = 16384;

LogDock::LogDock() : QDockWidget("Log")
{
	auto vbLayout = new QVBoxLayout(this);

	logMessageBox = new QTextEdit(this);
	logMessageBox->setReadOnly(true);

	vbLayout->addWidget(logMessageBox);

	clearButton = new QPushButton("Clear");
	clearButton->setMaximumWidth(75);
	connect(clearButton, &QPushButton::pressed, this, &LogDock::ClearLog);
	vbLayout->addWidget(clearButton);

	auto logWidget = new QWidget();
	logWidget->setLayout(vbLayout);

	setWidget(logWidget);

	setMaximumWidth(400);
}

void LogDock::Print(const std::wstring message)
{
	if (logMessageBox->toPlainText().length() > MAX_LOG_CHAR_LENGTH)
	{
		ClearLog();
	}

	logMessageBox->insertPlainText(QString::fromStdWString(message) + "\n");
	logMessageBox->moveCursor(QTextCursor::MoveOperation::End);
}

void LogDock::Print(const std::string message)
{
	if (logMessageBox->toPlainText().length() > MAX_LOG_CHAR_LENGTH)
	{
		ClearLog();
	}

	logMessageBox->insertPlainText(QString::fromStdString(message) + "\n");
	logMessageBox->moveCursor(QTextCursor::MoveOperation::End);
}

void LogDock::ClearLog()
{
	logMessageBox->clear();
}
