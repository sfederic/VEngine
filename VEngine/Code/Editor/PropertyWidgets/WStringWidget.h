#pragma once
import <string>;
#include <qlineedit.h>
#include "IPropertyWidget.h"

struct WStringWidget : public QLineEdit, IPropertyWidget
{
	std::wstring* value;

	WStringWidget(Property& value_);
	void SetValue();
	virtual void ResetValue() override;
};
