#include "IntWidget.h"

IntWidget::IntWidget(int* value, const char* name, QWidget* parent) : QSpinBox(parent)
{
	_value = value;

	connect(this, &QDoubleSpinBox::editingFinished, this, &IntWidget::SetValue);
}

void IntWidget::SetValue()
{
	if (_value)
	{
		*_value = value();
	}
}