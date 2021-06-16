#pragma once

#include <QDoubleSpinBox>
#include <DirectXMath.h>

using namespace DirectX;

class Float3Widget : public QDoubleSpinBox
{
public:
	Float3Widget(XMFLOAT3* value, QWidget* parent = 0);

private:
	void SetValue();

	QDoubleSpinBox xSpinbox;
	QDoubleSpinBox ySpinbox;
	QDoubleSpinBox zSpinbox;
	XMFLOAT3* _value;
};

