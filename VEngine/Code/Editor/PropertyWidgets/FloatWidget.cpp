#include "vpch.h"
#include "FloatWidget.h"
#include <QHBoxLayout>
#include <QLabel>

FloatWidget::FloatWidget(Property& prop_, QWidget* parent) : SpinBox(parent)
{
	prop = prop_;
	_value = prop.GetData<float>();
	connect(this, &SpinBox::editingFinished, this, &FloatWidget::SetValue);
	setValue(*_value);
}

void FloatWidget::SetValue()
{
	float val = (float)value();

	if (val != *_value)
	{
		IPropertyWidget::SetValue<float>(prop);
	}

	*_value = val;

	if (prop.change)
	{
		prop.change(prop);
	}
}

void FloatWidget::ResetValue()
{
	setValue(*_value);
}
