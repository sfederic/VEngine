#pragma once

#include <QDoubleSpinBox>

class FloatWidget : public QDoubleSpinBox
{
public:
	FloatWidget(float* value, const char* name, QWidget* parent = 0);

private:
	void SetValue();

	float* _value;
};