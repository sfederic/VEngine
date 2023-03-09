#include "vpch.h"
#include "Float2Widget.h"
#include <qboxlayout.h>
#include <DirectXMath.h>

using namespace DirectX;

Float2Widget::Float2Widget(Property& prop_, QWidget* parent) : QWidget(parent)
{
	prop = prop_;
	value = prop.GetData<XMFLOAT2>();

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
	IPropertyWidget::SetValue<XMFLOAT2>(prop);

	XMFLOAT2 newFloat3 = {};
	newFloat3.x = xSpinbox.value();
	newFloat3.y = ySpinbox.value();
	*value = newFloat3;

	IPropertyWidget::SetValue<XMFLOAT2>(prop);
}

void Float2Widget::ResetValue()
{
	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
}
