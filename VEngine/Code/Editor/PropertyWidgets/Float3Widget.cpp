#include "vpch.h"
#include "Float3Widget.h"
#include <qboxlayout.h>
#include <DirectXMath.h>
#include "Core/VMath.h"

using namespace DirectX;

Float3Widget::Float3Widget(Property& prop_, QWidget* parent) : QWidget(parent)
{
	prop = prop_;
	_value = prop.GetData<XMFLOAT3>();

	connect(&xSpinbox, &QDoubleSpinBox::editingFinished, this, &Float3Widget::SetValue);
	connect(&ySpinbox, &QDoubleSpinBox::editingFinished, this, &Float3Widget::SetValue);
	connect(&zSpinbox, &QDoubleSpinBox::editingFinished, this, &Float3Widget::SetValue);

	xSpinbox.setValue(_value->x);
	ySpinbox.setValue(_value->y);
	zSpinbox.setValue(_value->z);

	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(&xSpinbox);
	hLayout->addWidget(&ySpinbox);
	hLayout->addWidget(&zSpinbox);

	setLayout(hLayout);
}

void Float3Widget::SetValue()
{
	XMFLOAT3 newFloat3 = {};
	newFloat3.x = xSpinbox.value();
	newFloat3.y = ySpinbox.value();
	newFloat3.z = zSpinbox.value();

	if (!VMath::Float3Equal(newFloat3, *_value))
	{
		IPropertyWidget::SetValue<XMFLOAT3>(prop);
	}

	*_value = newFloat3;

	if (prop.change)
	{
		prop.change(_value);
	}
}

void Float3Widget::ResetValue()
{
	xSpinbox.setValue(_value->x);
	ySpinbox.setValue(_value->y);
	zSpinbox.setValue(_value->z);
}
