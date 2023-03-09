#include "vpch.h"
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
	*value_ = value();

	IPropertyWidget::SetValue<uint32_t>(prop);
}

void UintWidget::ResetValue()
{
	setValue(*value_);
}
