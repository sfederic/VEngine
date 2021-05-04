#include "FloatWidget.h"
#include <QHBoxLayout>
#include <QLabel>

FloatWidget::FloatWidget(float* value, const char* name, QWidget* parent) : QDoubleSpinBox(parent)
{
	_value = value;

	connect(this, &QDoubleSpinBox::editingFinished, this, &FloatWidget::SetValue);
}

void FloatWidget::SetValue()
{
	if (_value)
	{
		*_value = (float)value();
	}
}
