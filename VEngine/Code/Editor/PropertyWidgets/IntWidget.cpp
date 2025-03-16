
#include "IntWidget.h"

IntWidget::IntWidget(Property& prop_, bool readOnly, QWidget* parent) : IntSpinBox(parent)
{
	prop = prop_;
	_value = prop.GetData<int>();

	if (readOnly)
	{
		setDisabled(true);
	}

	connect(this, &QDoubleSpinBox::editingFinished, this, &IntWidget::SetValue);
	setValue(*_value);
}

void IntWidget::SetValue()
{
	const int val = value();

	if (val != *_value)
	{
		IPropertyWidget::SetValue<int>(prop);
	}

	*_value = value();

	if (prop.change)
	{
		prop.change(prop);
	}
}

void IntWidget::ResetValue()
{
	setValue(*_value);
}
