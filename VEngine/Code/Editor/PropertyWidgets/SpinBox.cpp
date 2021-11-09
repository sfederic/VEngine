#define NOMINMAX
#include "SpinBox.h"

SpinBox::SpinBox(QWidget* parent) : QDoubleSpinBox(parent)
{
	setMinimum(std::numeric_limits<double>::lowest());
	setMaximum(std::numeric_limits<double>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);

	//TODO: If you remove this, the entire editor becomes minimised and you have to click maximise twice
	//to get back to fullscreen. Try figure it out.
	setFixedWidth(75);

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
