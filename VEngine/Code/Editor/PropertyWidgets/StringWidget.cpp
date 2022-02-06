#include "StringWidget.h"
#include "Properties.h"
#include <QDir>
#include <QStringListModel>
#include <QCompleter>

StringWidget::StringWidget(Property value_)
{
	prop = value_;

	value = (std::string*)value_.data;
	setText(QString::fromStdString(value->data()));

	connect(this, &QLineEdit::editingFinished, this, &StringWidget::SetValue);

	//Set auto complete
	if (!prop.autoCompletePath.empty())
	{
		QString path = QDir::currentPath() + QString::fromStdString(prop.autoCompletePath);
		QDir dir(path);
		QStringList dirContents = dir.entryList(QStringList(), QDir::Files);

		auto model = new QStringListModel(dirContents, this);

		QCompleter* fileEditCompleter = new QCompleter(model, this);
		fileEditCompleter->setCaseSensitivity(Qt::CaseInsensitive);
		fileEditCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
		this->setCompleter(fileEditCompleter);
	}
}

void StringWidget::SetValue()
{
	QString txt = text();
	value->assign(txt.toStdString().c_str());

	if (prop.change)
	{
		prop.change(value);
	}

	clearFocus();
}

void StringWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdString(value->data()));
	}
}
