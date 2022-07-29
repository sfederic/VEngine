#pragma once

#include <qspinbox.h>

class QWheelEvent;

struct SpinBox : public QDoubleSpinBox
{
	SpinBox(QWidget* parent = nullptr);
	void wheelEvent(QWheelEvent* event_) override;
};

struct IntSpinBox : public QSpinBox
{
	IntSpinBox(QWidget* parent = nullptr);
	void wheelEvent(QWheelEvent* event_) override;
};

struct UintSpinBox : public QSpinBox
{
	UintSpinBox(QWidget* parent = nullptr);
	void wheelEvent(QWheelEvent* event_) override;
};
