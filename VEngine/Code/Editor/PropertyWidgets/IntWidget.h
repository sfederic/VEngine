#pragma once

#include <QDoubleSpinBox>
#include "IPropertyWidget.h"

class IntWidget : public QSpinBox, IPropertyWidget
{
public:
	IntWidget(int* value, QWidget* parent = 0);

private:
	void SetValue();
	virtual void ResetValue() override;

	int* _value;
};
