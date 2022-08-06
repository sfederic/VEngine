#pragma once
#include "IPropertyWidget.h"
#include <qlineedit.h>

struct ShaderData;

class ShaderDataWidget : public QLineEdit, IPropertyWidget
{
private:
	ShaderData* value;
	Property prop;

public:
	ShaderDataWidget(Property& prop_);
	void SetValue();
	virtual void ResetValue() override;
};
