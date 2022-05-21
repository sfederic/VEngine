#include "vpch.h"
#include "UintWidget.h"

UintWidget::UintWidget(uint32_t* value, bool readOnly, QWidget* parent) : UintSpinBox(parent)
{
	value_ = value;

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
}

void UintWidget::ResetValue()
{
	setValue(*value_);
}
