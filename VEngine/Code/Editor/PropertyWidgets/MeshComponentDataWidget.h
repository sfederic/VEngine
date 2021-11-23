#pragma once
#include <qpushbutton.h>
#include "IPropertyWidget.h"
#include "Properties.h"

struct MeshComponentData;

struct MeshComponentDataWidget : public QPushButton, IPropertyWidget
{
	MeshComponentData* value;
	Property prop;

	MeshComponentDataWidget(Property prop_);
	void SetValue();
	virtual void ResetValue() override;
};
