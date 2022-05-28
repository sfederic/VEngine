#pragma once
#include "SpinBox.h"
#include "IPropertyWidget.h"

struct FloatWidget : SpinBox, IPropertyWidget
{
	FloatWidget(Property& prop_, QWidget* parent = 0);

	void SetValue();
	virtual void ResetValue() override;

	float* _value = nullptr;
};