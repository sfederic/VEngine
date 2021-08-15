#pragma once

#include <qwidget.h>
#include <DirectXMath.h>

using namespace DirectX;

class QDoubleSpinBox;

class VectorWidget : public QWidget
{
public: 
	VectorWidget(XMVECTOR* vector, QWidget* parent = 0);
private:
	void SetValue();

	QDoubleSpinBox* xSpinbox;
	QDoubleSpinBox* ySpinbox;
	QDoubleSpinBox* zSpinbox;
	XMVECTOR* _vector;
};
