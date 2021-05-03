#pragma once

#include <QDoubleSpinBox>

class IntWidget : public QWidget
{
public:
	IntWidget(int* value, const char* name, QWidget* parent = 0);

private:
	void SetValue();

	QSpinBox* spinbox;
	int* _value;
};
