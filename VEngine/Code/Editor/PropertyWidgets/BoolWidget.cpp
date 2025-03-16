
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

	connect(this, &QCheckBox::stateChanged, this, &BoolWidget::SetValueBool);
}

void BoolWidget::SetValueBool(int state)
{
	if (value)
	{
		if (*value != (bool)state)
		{
			IPropertyWidget::SetValue<bool>(prop);
		}

		*value = state;

		if (prop.change)
		{
			prop.change(prop);
		}
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
