#pragma once
#include <qcombobox.h>
#include "IPropertyWidget.h"

struct VEnum;

struct VEnumWidget : public QComboBox, IPropertyWidget
{
	VEnum* value = nullptr;

	VEnumWidget(Property value_);
	void SetValue();
	virtual void ResetValue() override;
};
