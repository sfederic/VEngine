#include "IntWidget.h"
#include <QHBoxLayout>
#include <QLabel>

IntWidget::IntWidget(int* value, const char* name, QWidget* parent) : QWidget(parent)
{
	_value = value;

	auto layout = new QHBoxLayout();
	setLayout(layout);

	//Label
	auto label = new QLabel(name);
	layout->addWidget(label);

	//Spinbox
	spinbox = new QSpinBox();
	connect(spinbox, &QDoubleSpinBox::editingFinished, this, &IntWidget::SetValue);
	layout->addWidget(spinbox);
}

void IntWidget::SetValue()
{
	if (_value)
	{
		*_value = spinbox->value();
	}
}