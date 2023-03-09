#pragma once

#include <qcheckbox.h>
#include "IPropertyWidget.h"

class BoolWidget : public QCheckBox, public IPropertyWidget
{
public:
	BoolWidget(Property& prop, QWidget* parent = 0);

private:
	void SetValueBool(int state);
	virtual void ResetValue() override;

	bool* value;
};
