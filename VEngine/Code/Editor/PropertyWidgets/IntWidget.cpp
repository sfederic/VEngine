#include "IntWidget.h"

IntWidget::IntWidget(int* value, QWidget* parent) : SpinBox(parent)
{
	_value = value;

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
