#pragma once

#include "SpinBox.h"
#include "IPropertyWidget.h"

class IntWidget : public SpinBox, IPropertyWidget
{
public:
	IntWidget(int* value, QWidget* parent = 0);

private:
	void SetValue();
	virtual void ResetValue() override;

	int* _value;
};
