#include "StringWidget.h"

StringWidget::StringWidget(std::string* value_)
{
	value = value_;
	setText(QString::fromStdString(value->data()));
	connect(this, &QLineEdit::editingFinished, this, &StringWidget::SetValue);
}

void StringWidget::SetValue()
{
	QString txt = text();
	value->assign(txt.toStdString());
}

void StringWidget::ResetValue()
{
	setText(QString::fromStdString(value->data()));
}
