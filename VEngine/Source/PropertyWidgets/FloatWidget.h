#pragma once

#include <QDoubleSpinBox>

class FloatWidget : public QDoubleSpinBox
{
public:
	FloatWidget(float value, QWidget* parent = 0);

private:
	void SetValue();

	float* _value;
};