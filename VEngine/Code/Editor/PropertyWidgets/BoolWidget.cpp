#include "vpch.h"
#include "BoolWidget.h"

BoolWidget::BoolWidget(Property& prop_, QWidget* parent) : QCheckBox(parent)
{
	prop = prop_;
	value = prop.GetData<bool>();

	if (*value == true)
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
	if (value)
	{
		*value = value_;
	}
}

void BoolWidget::ResetValue()
{
	if (*value == true)
	{
		setCheckState(Qt::CheckState::Checked);
	}
	else
	{
		setCheckState(Qt::CheckState::Unchecked);
	}
}
