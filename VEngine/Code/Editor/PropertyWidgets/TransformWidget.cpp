#include "TransformWidget.h"
#include <qboxlayout.h>
#include "Float3Widget.h"
#include "Float4Widget.h"
#include "Transform.h"

TransformWidget::TransformWidget(Transform* value_, QWidget* parent) : QWidget(parent)
{
	value = value_;

	positionWidget = new Float3Widget(&value->position, this);
	scaleWidget = new Float3Widget(&value->scale, this);
	rotationWidget = new Float4Widget(&value->rotation, this);

	auto vLayout = new QVBoxLayout(this);
	vLayout->addWidget(positionWidget);
	vLayout->addWidget(scaleWidget);
	vLayout->addWidget(rotationWidget);

	setLayout(vLayout);
}

void TransformWidget::SetValue()
{
	Transform newTransform = {};
	newTransform.scale = *scaleWidget->_value;
	newTransform.position = *positionWidget->_value;
	newTransform.rotation = *rotationWidget->value;

	value->position = newTransform.position;
	value->scale = newTransform.scale;
	value->rotation = newTransform.rotation;
}

void TransformWidget::ResetValue()
{
	positionWidget->ResetValue();
	scaleWidget->ResetValue();
	rotationWidget->ResetValue();
}
