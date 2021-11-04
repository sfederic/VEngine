#pragma once
#include <qlineedit.h>
#include "IPropertyWidget.h"

struct Material;

struct MaterialWidget : public QLineEdit, IPropertyWidget
{
	Material* material = nullptr;

	MaterialWidget(Material* material_);
	void SetValue();
	virtual void ResetValue() override;
};
