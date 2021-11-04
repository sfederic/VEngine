#pragma once
#include <qlineedit.h>
#include "IPropertyWidget.h"

struct Material;

struct MaterialWidget : public QLineEdit, IPropertyWidget
{
	Material* material = nullptr;

	MaterialWidget(Material* material_, QWidget* parent = nullptr);
	void SetValue();
	virtual void ResetValue() override;
};
