#include "vpch.h"
#include "Float2Widget.h"
#include <qboxlayout.h>
#include <DirectXMath.h>
#include "Core/VMath.h"

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

	XMFLOAT2 newFloat2 = {};
	newFloat2.x = xSpinbox.value();
	newFloat2.y = ySpinbox.value();

	if (!VMath::Float2Equal(newFloat2, *value))
	{
		IPropertyWidget::SetValue<XMFLOAT2>(prop);
	}

	*value = newFloat2;

	if (prop.change)
	{
		prop.change(value);
	}
}

void Float2Widget::ResetValue()
{
	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
}
