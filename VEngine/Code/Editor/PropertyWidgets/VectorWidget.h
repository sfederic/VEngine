#pragma once

#include <qwidget.h>
#include <DirectXMath.h>
#include "IPropertyWidget.h"

using namespace DirectX;

class QDoubleSpinBox;

class VectorWidget : public QWidget, IPropertyWidget
{
public: 
	VectorWidget(XMVECTOR* vector, QWidget* parent = 0);
private:
	void SetValue();
	virtual void ResetValue() override;

	QDoubleSpinBox* xSpinbox;
	QDoubleSpinBox* ySpinbox;
	QDoubleSpinBox* zSpinbox;
	XMVECTOR* _vector;
};
