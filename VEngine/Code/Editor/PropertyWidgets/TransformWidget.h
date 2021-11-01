#pragma once
#include "IPropertyWidget.h"
#include <qwidget.h>

class Float3Widget;
struct Float4Widget;
struct Transform;

struct TransformWidget : public QWidget, IPropertyWidget
{
	Float3Widget* positionWidget = nullptr;
	Float3Widget* scaleWidget = nullptr;
	Float4Widget* rotationWidget = nullptr;

	Transform* value = nullptr;

	TransformWidget(Transform* value_, QWidget* parent = nullptr);
	void SetValue();
	virtual void ResetValue() override;
};
