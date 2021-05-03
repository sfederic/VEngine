#pragma once

#include <QDoubleSpinBox>

class FloatWidget : public QWidget
{
public:
	FloatWidget(float* value, const char* name, QWidget* parent = 0);

private:
	void SetValue();

	QDoubleSpinBox* spinbox;
	float* _value;
};