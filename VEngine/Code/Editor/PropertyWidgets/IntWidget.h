#pragma once

#include "SpinBox.h"
#include "IPropertyWidget.h"

class IntWidget : public IntSpinBox, IPropertyWidget
{
public:
	IntWidget(int* value, bool readOnly = false, QWidget* parent = 0);

private:
	void SetValue();
	virtual void ResetValue() override;

	int* _value;
};
