#pragma once

#include <qcheckbox.h>
#include "IPropertyWidget.h"

class BoolWidget : public QCheckBox, IPropertyWidget
{
public:
	BoolWidget(bool* value, QWidget* parent = 0);

private:
	void SetValue(int value);
	virtual void ResetValue() override;

	bool* _value;
};
