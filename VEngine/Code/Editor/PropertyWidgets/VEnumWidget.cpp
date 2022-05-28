#include "vpch.h"
#include "VEnumWidget.h"
#include "VEnum.h"

VEnumWidget::VEnumWidget(Property& value_)
{
	prop = value_;
	value = value_.GetData<VEnum>();

	for (auto& name : value->GetAllNames())
	{
		addItem(QString::fromStdString(name));
	}

	setCurrentText(QString::fromStdString(value->GetValue()));

	connect(this, &QComboBox::currentTextChanged, this, &VEnumWidget::SetValue);
}

void VEnumWidget::SetValue()
{
	std::string text = currentText().toStdString();
	value->SetValue(text);
	if (prop.change)
	{
		prop.change(value);
	}

	clearFocus();
}

void VEnumWidget::ResetValue()
{
}
