#pragma once
#include "IPropertyWidget.h"
#include <qlineedit.h>

struct ShaderData;

class ShaderDataWidget : public QLineEdit, public IPropertyWidget
{
private:
	ShaderData* value;
	Property prop;

public:
	ShaderDataWidget(Property& prop_);
	void SetValue();
	virtual void ResetValue() override;
};
