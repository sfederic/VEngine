#include "WStringWidget.h"
#include "Properties.h"

WStringWidget::WStringWidget(Property value_)
{
	prop = value_;

	value = (std::wstring*)value_.data;
	setText(QString::fromStdWString(value->data()));

	connect(this, &QLineEdit::editingFinished, this, &WStringWidget::SetValue);
}

void WStringWidget::SetValue()
{
	QString txt = text();
	value->assign(txt.toStdWString());

	if (prop.change)
	{
		prop.change(value);
	}

	clearFocus();
}

void WStringWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdWString(value->data()));
	}
}
