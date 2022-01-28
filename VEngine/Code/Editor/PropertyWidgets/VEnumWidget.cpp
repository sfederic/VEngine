#include "VEnumWidget.h"
#include "VEnum.h"

VEnumWidget::VEnumWidget(Property value_)
{
	prop = value_;
	value = (VEnum*)value_.data;

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
}

void VEnumWidget::ResetValue()
{
}
