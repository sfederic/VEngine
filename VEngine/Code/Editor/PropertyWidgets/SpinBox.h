#pragma once
#include <qspinbox.h>

struct SpinBox : public QDoubleSpinBox
{
	SpinBox(QWidget* parent = nullptr);
};
