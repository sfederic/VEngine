#pragma once
#include "IPropertyWidget.h"
#include <qpushbutton.h>
#include "Properties.h"

struct ShaderData;

struct ShaderDataWidget : public QPushButton, IPropertyWidget
{
	ShaderData* value;
	Property prop;

	ShaderDataWidget(Property& prop_);
	void SetValue();
	virtual void ResetValue() override;
};
