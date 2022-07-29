#pragma once
#include "SpinBox.h"
#include "IPropertyWidget.h"

struct DirectX::XMFLOAT2;

struct Float2Widget : public QWidget, IPropertyWidget
{
	SpinBox xSpinbox;
	SpinBox ySpinbox;
	DirectX::XMFLOAT2* value;

	Float2Widget(Property& prop_, QWidget* parent = 0);
	void SetValue();
	virtual void ResetValue() override;
};
