#pragma once

#include "SpinBox.h"
#include "IPropertyWidget.h"

class IntWidget : public IntSpinBox, public IPropertyWidget
{
public:
	IntWidget(Property& prop_, bool readOnly = false, QWidget* parent = 0);

private:
	void SetValue();
	virtual void ResetValue() override;

	int* _value = nullptr;
};
