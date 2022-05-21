#include "vpch.h"
#include "IntWidget.h"

IntWidget::IntWidget(int* value, bool readOnly, QWidget* parent) : IntSpinBox(parent)
{
	_value = value;

	if (readOnly)
	{
		setDisabled(true);
	}

	connect(this, &QDoubleSpinBox::editingFinished, this, &IntWidget::SetValue);
	setValue(*_value);
}

void IntWidget::SetValue()
{
	*_value = value();
}

void IntWidget::ResetValue()
{
	setValue(*_value);
}
