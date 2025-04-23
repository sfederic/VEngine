#include <QWidget>
#include <qboxlayout.h>
#include <DirectXMath.h>

export module Editor.PropertyWidgets.Float2Widget;

import Editor.PropertyWidgets.IPropertyWidget;
import Editor.PropertyWidgets.SpinBox;
import Core.Property;
import Core.VMath;

export struct Float2Widget : public QWidget, public IPropertyWidget
{
	SpinBox xSpinbox;
	SpinBox ySpinbox;
	DirectX::XMFLOAT2* value;

	Float2Widget(Property& prop_, QWidget* parent = 0);
	void SetValue();
	virtual void ResetValue() override;
};

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
		prop.change(prop);
	}
}

void Float2Widget::ResetValue()
{
	xSpinbox.setValue(value->x);
	ySpinbox.setValue(value->y);
}
