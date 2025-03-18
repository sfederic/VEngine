#include <qcheckbox.h>

export module Editor.PropertyWidgets.BoolWidget;

import Editor.PropertyWidgets.IPropertyWidget;
import Core.Property;

export class BoolWidget : public QCheckBox, public IPropertyWidget
{
public:
	BoolWidget(Property& prop, QWidget* parent = 0);

private:
	void SetValueBool(int state);
	virtual void ResetValue() override;

	bool* value;
};

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
