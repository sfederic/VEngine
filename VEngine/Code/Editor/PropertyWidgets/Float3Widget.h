#pragma once

#include "SpinBox.h"
#include "IPropertyWidget.h"

struct DirectX::XMFLOAT3;

class Float3Widget : public QWidget, IPropertyWidget
{
private:
	SpinBox xSpinbox;
	SpinBox ySpinbox;
	SpinBox zSpinbox;
	DirectX::XMFLOAT3* _value = nullptr;

public:
	Float3Widget(Property& prop_, QWidget* parent = 0);
	void SetValue();
	virtual void ResetValue() override;
};
