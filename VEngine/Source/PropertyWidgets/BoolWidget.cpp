#include "BoolWidget.h"

BoolWidget::BoolWidget(bool* value, QWidget* parent) : QCheckBox(parent)
{
	_value = value;
	if (*_value == true)
	{
		setCheckState(Qt::CheckState::Checked);
	}
	else
	{
		setCheckState(Qt::CheckState::Unchecked);
	}

	connect(this, &QCheckBox::stateChanged, this, &BoolWidget::SetValue);
}

void BoolWidget::SetValue(int value_)
{
	if (_value)
	{
		*_value = value_;
	}
}
