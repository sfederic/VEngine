#pragma once

#include <qcheckbox.h>
#include "IPropertyWidget.h"

class BoolWidget : public QCheckBox, public IPropertyWidget
{
public:
	BoolWidget(Property& prop, QWidget* parent = 0);

private:
	void SetValue(int value);
	virtual void ResetValue() override;

	bool* value;
};
