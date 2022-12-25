#pragma once

#include "SpinBox.h"
#include "IPropertyWidget.h"

class Float4Widget : public QWidget, public IPropertyWidget
{
private:
	SpinBox xSpinbox;
	SpinBox ySpinbox;
	SpinBox zSpinbox;
	SpinBox wSpinbox;
	DirectX::XMFLOAT4* value = nullptr;

public:
	Float4Widget(Property& prop_, QWidget* parent = 0);
	void SetValue();
	virtual void ResetValue() override;
};

