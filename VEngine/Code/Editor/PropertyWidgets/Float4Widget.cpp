
#include "Float4Widget.h"
#include <qboxlayout.h>
import <DirectXMath.h>;
import Core.VMath;

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
	IPropertyWidget::SetValue<XMFLOAT4>(prop);

	XMFLOAT4 newFloat4 = {};
	newFloat4.x = xSpinbox.value();
	newFloat4.y = ySpinbox.value();
	newFloat4.z = zSpinbox.value();
	newFloat4.w = wSpinbox.value();

	if (!VMath::Float4Equal(newFloat4, *value))
	{
		IPropertyWidget::SetValue<XMFLOAT4>(prop);
	}

	*value = newFloat4;

	if (prop.change)
	{
		prop.change(prop);
	}
}

void Float4Widget::ResetValue()
{
	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
	zSpinbox.setValue(value->z);
	wSpinbox.setValue(value->w);
}
