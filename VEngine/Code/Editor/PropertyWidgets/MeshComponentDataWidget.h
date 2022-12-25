#pragma once

#include <qpushbutton.h>
#include "IPropertyWidget.h"

struct MeshComponentData;

class MeshComponentDataWidget : public QPushButton, public IPropertyWidget
{
private:
	MeshComponentData* value = nullptr;
	Property prop;

public:
	MeshComponentDataWidget(Property& prop_);
	void SetValue();
	virtual void ResetValue() override;
};
