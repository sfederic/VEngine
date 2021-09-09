#include "Float4Widget.h"
#include <qboxlayout.h>

Float4Widget::Float4Widget(XMFLOAT4* value_, QWidget* parent) : QWidget(parent)
{
	value = value_;

	connect(&xSpinbox, &QDoubleSpinBox::editingFinished, this, &Float4Widget::SetValue);
	connect(&ySpinbox, &QDoubleSpinBox::editingFinished, this, &Float4Widget::SetValue);

	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
	zSpinbox.setValue(value->z);
	wSpinbox.setValue(value->w);

	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(&xSpinbox);
	hLayout->addWidget(&ySpinbox);
	hLayout->addWidget(&zSpinbox);
	hLayout->addWidget(&wSpinbox);

	setLayout(hLayout);
}

void Float4Widget::SetValue()
{
	XMFLOAT4 newFloat4 = {};
	newFloat4.x = xSpinbox.value();
	newFloat4.y = ySpinbox.value();
	newFloat4.z = zSpinbox.value();
	newFloat4.w = zSpinbox.value();
	*value = newFloat4;
}

void Float4Widget::ResetValue()
{
	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
	zSpinbox.setValue(value->z);
	wSpinbox.setValue(value->w);
}
