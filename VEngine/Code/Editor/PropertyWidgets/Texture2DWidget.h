#pragma once
#include "IPropertyWidget.h"
#include <qpushbutton.h>
#include "Properties.h"

struct Texture2D;

struct Texture2DWidget : QPushButton, IPropertyWidget
{
	Texture2D* value;
	Property prop;

	Texture2DWidget(Property prop_);
	void SetValue();
	virtual void ResetValue() override;
};
