#pragma once
#include "IPropertyWidget.h"
#include "SpinBox.h"

struct UintWidget : UintSpinBox, IPropertyWidget
{
	uint32_t* value_ = nullptr;

	UintWidget(Property& prop, bool readOnly = false, QWidget* parent = nullptr);
	void SetValue();
	virtual void ResetValue() override;
};
