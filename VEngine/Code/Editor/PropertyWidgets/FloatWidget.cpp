#include "vpch.h"
#include "FloatWidget.h"
#include <QHBoxLayout>
#include <QLabel>

FloatWidget::FloatWidget(float* value, QWidget* parent) : SpinBox(parent)
{
	_value = value;
	connect(this, &SpinBox::editingFinished, this, &FloatWidget::SetValue);
	setValue(*_value);
}

void FloatWidget::SetValue()
{
	*_value = (float)value();
}

void FloatWidget::ResetValue()
{
	setValue(*_value);
}
