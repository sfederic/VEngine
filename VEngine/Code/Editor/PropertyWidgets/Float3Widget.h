#pragma once
#include <QDoubleSpinBox>
#include <DirectXMath.h>
#include "IPropertyWidget.h"

using namespace DirectX;

class Float3Widget : public QWidget, IPropertyWidget
{
public:
	Float3Widget(XMFLOAT3* value, QWidget* parent = 0);
	void SetValue();
	virtual void ResetValue() override;

private:
	QDoubleSpinBox xSpinbox;
	QDoubleSpinBox ySpinbox;
	QDoubleSpinBox zSpinbox;
	XMFLOAT3* _value;
};
