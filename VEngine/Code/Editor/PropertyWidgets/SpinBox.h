#pragma once
#include <qspinbox.h>

struct SpinBox : public QDoubleSpinBox
{
	SpinBox(QWidget* parent = nullptr);
};

struct IntSpinBox : public QSpinBox
{
	IntSpinBox(QWidget* parent = nullptr);
};

struct UintSpinBox : public QSpinBox
{
	UintSpinBox(QWidget* parent = nullptr);
};
