#include "SpinBox.h"

SpinBox::SpinBox(QWidget* parent) : QDoubleSpinBox(parent)
{
	setMinimum(std::numeric_limits<double>::lowest());
	setMaximum(std::numeric_limits<double>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);
	setFixedWidth(75);
	setDecimals(3);
}
