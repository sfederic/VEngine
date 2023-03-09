#include "vpch.h"
#include "Float4Widget.h"
#include <qboxlayout.h>
#include <DirectXMath.h>

using namespace DirectX;

Float4Widget::Float4Widget(Property& prop_, QWidget* parent) : QWidget(parent)
{
	prop = prop_;
	value = prop.GetData<XMFLOAT4>();

	connect(&xSpinbox, &QDoubleSpinBox::editingFinished, this, &Float4Widget::SetValue);
	connect(&ySpinbox, &QDoubleSpinBox::editingFinished, this, &Float4Widget::SetValue);
	connect(&zSpinbox, &QDoubleSpinBox::editingFinished, this, &Float4Widget::SetValue);
	connect(&wSpinbox, &QDoubleSpinBox::editingFinished, this, &Float4Widget::SetValue);

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
	newFloat4.w = wSpinbox.value();
	*value = newFloat4;

	IPropertyWidget::SetValue<XMFLOAT4>(prop);
}

void Float4Widget::ResetValue()
{
	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
	zSpinbox.setValue(value->z);
	wSpinbox.setValue(value->w);
}
