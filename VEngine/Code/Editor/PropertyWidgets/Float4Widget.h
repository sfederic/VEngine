#pragma once
#include "SpinBox.h"
#include <DirectXMath.h>
#include "IPropertyWidget.h"

using namespace DirectX;

struct Float4Widget : public QWidget, IPropertyWidget
{
	SpinBox xSpinbox;
	SpinBox ySpinbox;
	SpinBox zSpinbox;
	SpinBox wSpinbox;
	XMFLOAT4* value;

	Float4Widget(Property& prop_, QWidget* parent = 0);
	void SetValue();
	virtual void ResetValue() override;
};

