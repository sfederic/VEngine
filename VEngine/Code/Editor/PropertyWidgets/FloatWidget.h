#pragma once

#include <QDoubleSpinBox>
#include "IPropertyWidget.h"

class FloatWidget : public QDoubleSpinBox, IPropertyWidget
{
public:
	FloatWidget(float value, QWidget* parent = 0);

private:
	void SetValue();
	virtual void ResetValue() override;

	float* _value;
};