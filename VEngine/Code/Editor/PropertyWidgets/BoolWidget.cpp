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

	connect(this, &QCheckBox::stateChanged, this, &BoolWidget::SetValueBool);
}

void BoolWidget::SetValueBool(int state)
{
	if (value)
	{
		IPropertyWidget::SetValue<bool>(prop);

		*value = state;
		SetValue<bool>(prop);

		IPropertyWidget::SetValue<bool>(prop);
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
