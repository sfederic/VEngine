#pragma once
#include "IPropertyWidget.h"
#include <qpushbutton.h>

struct ShaderData;

class ShaderDataWidget : public QPushButton, IPropertyWidget
{
private:
	ShaderData* value;
	Property prop;

public:
	ShaderDataWidget(Property& prop_);
	void SetValue();
	virtual void ResetValue() override;
};
