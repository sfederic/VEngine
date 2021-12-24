#include "StringWidget.h"
#include "Properties.h"

StringWidget::StringWidget(Property value_)
{
	prop = value_;

	value = (std::string*)value_.data;
	setText(QString::fromStdString(value->data()));

	connect(this, &QLineEdit::editingFinished, this, &StringWidget::SetValue);
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
