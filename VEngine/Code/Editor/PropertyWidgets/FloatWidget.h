#pragma once
#include "SpinBox.h"
#include "IPropertyWidget.h"

class FloatWidget : SpinBox, IPropertyWidget
{
public:
	FloatWidget(float* value, QWidget* parent = 0);

	void SetValue();
	virtual void ResetValue() override;

	float* _value;
};