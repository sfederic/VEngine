
#include "SpinBox.h"
#include <QWheelEvent>

SpinBox::SpinBox(QWidget* parent) : QDoubleSpinBox(parent)
{
	setMinimum(std::numeric_limits<double>::lowest());
	setMaximum(std::numeric_limits<double>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);

	setDecimals(3);

	setFocusPolicy(Qt::StrongFocus);
}

void SpinBox::wheelEvent(QWheelEvent* event_)
{
	!hasFocus() ? event_->ignore() : QDoubleSpinBox::wheelEvent(event_);
}

IntSpinBox::IntSpinBox(QWidget* parent) : QSpinBox(parent)
{
	setMinimum(std::numeric_limits<int>::lowest());
	setMaximum(std::numeric_limits<int>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);

	setFocusPolicy(Qt::StrongFocus);
}

void IntSpinBox::wheelEvent(QWheelEvent* event_)
{
	!hasFocus() ? event_->ignore() : QSpinBox::wheelEvent(event_);
}

UintSpinBox::UintSpinBox(QWidget* parent) : QSpinBox(parent)
{
	setMinimum(std::numeric_limits<uint32_t>::lowest());
	setMaximum(std::numeric_limits<uint32_t>::max());

	setButtonSymbols(QAbstractSpinBox::NoButtons);

	setFocusPolicy(Qt::StrongFocus);
}

void UintSpinBox::wheelEvent(QWheelEvent* event_)
{
	!hasFocus() ? event_->ignore() : QSpinBox::wheelEvent(event_);
}
