
#include "UintWidget.h"

UintWidget::UintWidget(Property& prop_, bool readOnly, QWidget* parent) : UintSpinBox(parent)
{
	prop = prop_;
	value_ = prop.GetData<uint32_t>();

	if (readOnly)
	{
		setDisabled(true);
	}

	connect(this, &UintSpinBox::editingFinished, this, &UintWidget::SetValue);
	ResetValue();
}

void UintWidget::SetValue()
{
	const uint32_t val = value();

	if (val != *value_)
	{
		IPropertyWidget::SetValue<uint32_t>(prop);
	}

	*value_ = val;

	if (prop.change)
	{
		prop.change(prop);
	}
}

void UintWidget::ResetValue()
{
	setValue(*value_);
}
