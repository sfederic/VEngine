
#include "WStringWidget.h"

WStringWidget::WStringWidget(Property& value_)
{
	prop = value_;

	value = (std::wstring*)value_.data;
	setText(QString::fromStdWString(value->data()));

	connect(this, &QLineEdit::editingFinished, this, &WStringWidget::SetValue);
}

void WStringWidget::SetValue()
{
	std::wstring txt = text().toStdWString();

	if (*value != txt)
	{
		IPropertyWidget::SetValue<std::wstring>(prop);
	}

	value->assign(txt);

	if (prop.change)
	{
		prop.change(prop);
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
