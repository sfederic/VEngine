#pragma once

#include <QDoubleSpinBox>

class IntWidget : public QSpinBox
{
public:
	IntWidget(int* value, QWidget* parent = 0);

private:
	void SetValue();

	int* _value;
};
