#include "FloatWidget.h"
#include <QHBoxLayout>
#include <QLabel>

FloatWidget::FloatWidget(float* value, const char* name, QWidget* parent) : QWidget(parent)
{
	_value = value;

	auto layout = new QHBoxLayout();
	setLayout(layout);

	//Label
	auto label = new QLabel(name);
	layout->addWidget(label);

	//DoubleSpinbox
	spinbox = new QDoubleSpinBox();
	connect(spinbox, &QDoubleSpinBox::editingFinished, this, &FloatWidget::SetValue);
	layout->addWidget(spinbox);
}

void FloatWidget::SetValue()
{
	if (_value)
	{
		*_value = (float)spinbox->value();
	}
}
