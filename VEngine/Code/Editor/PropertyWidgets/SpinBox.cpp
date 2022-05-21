#include "vpch.h"
#include "SpinBox.h"

SpinBox::SpinBox(QWidget* parent) : QDoubleSpinBox(parent)
{
	setMinimum(std::numeric_limits<double>::lowest());
	setMaximum(std::numeric_limits<double>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);

	setDecimals(3);
}

IntSpinBox::IntSpinBox(QWidget* parent)
{
	setMinimum(std::numeric_limits<int>::lowest());
	setMaximum(std::numeric_limits<int>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);
}

UintSpinBox::UintSpinBox(QWidget* parent)
{
	setMinimum(std::numeric_limits<uint32_t>::lowest());
	setMaximum(std::numeric_limits<uint32_t>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);
}
