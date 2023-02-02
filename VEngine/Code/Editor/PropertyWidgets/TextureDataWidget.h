#pragma once

#include "IPropertyWidget.h"
#include <qpushbutton.h>
#include "Core/Properties.h"

struct TextureData;

struct TextureDataWidget : public QPushButton, IPropertyWidget
{
	TextureData* value;
	Property prop;

	TextureDataWidget(Property& prop_);
	void SetValue();
	virtual void ResetValue() override;
};
