#include "Float2Widget.h"
#include <qboxlayout.h>

Float2Widget::Float2Widget(XMFLOAT2* value_, QWidget* parent) : QWidget(parent)
{
	value = value_;

	connect(&xSpinbox, &QDoubleSpinBox::editingFinished, this, &Float2Widget::SetValue);
	connect(&ySpinbox, &QDoubleSpinBox::editingFinished, this, &Float2Widget::SetValue);

	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);

	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(&xSpinbox);
	hLayout->addWidget(&ySpinbox);

	setLayout(hLayout);
}

void Float2Widget::SetValue()
{
	XMFLOAT2 newFloat3 = {};
	newFloat3.x = xSpinbox.value();
	newFloat3.y = ySpinbox.value();
	*value = newFloat3;
}

void Float2Widget::ResetValue()
{
	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
}
