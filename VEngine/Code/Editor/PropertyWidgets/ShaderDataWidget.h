#pragma once

#include "IPropertyWidget.h"
#include <qlineedit.h>

struct ShaderData;

class ShaderDataWidget : public QLineEdit, public IPropertyWidget
{
public:
	ShaderDataWidget(Property& prop_);
	void SetValue();
	virtual void ResetValue() override;

private:
	ShaderData* value = nullptr;
	Property prop;
};
