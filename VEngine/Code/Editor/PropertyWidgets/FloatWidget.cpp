#include "FloatWidget.h"
#include <QHBoxLayout>
#include <QLabel>

FloatWidget::FloatWidget(float value, QWidget* parent) : QDoubleSpinBox(parent)
{
	//_value = value;

	setDecimals(4);
	setMinimum(std::numeric_limits<float>::lowest());
	setMaximum(std::numeric_limits<double>::max());
	setButtonSymbols(QAbstractSpinBox::NoButtons);

	//connect(this, &QDoubleSpinBox::editingFinished, this, &FloatWidget::SetValue);

	setValue(value);
}

void FloatWidget::SetValue()
{
	*_value = (float)value();
}

void FloatWidget::ResetValue()
{
	setValue(*_value);
}
