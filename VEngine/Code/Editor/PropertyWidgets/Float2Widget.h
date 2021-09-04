#pragma once
#include "SpinBox.h"
#include <DirectXMath.h>
#include "IPropertyWidget.h"

using namespace DirectX;

struct Float2Widget : public QWidget, IPropertyWidget
{
	SpinBox xSpinbox;
	SpinBox ySpinbox;
	XMFLOAT2* value;

	Float2Widget(XMFLOAT2* value_, QWidget* parent = 0);
	void SetValue();
	virtual void ResetValue() override;
};
