#include "VectorWidget.h"
#include <QHBoxLayout>
#include <QDoubleSpinBox>

VectorWidget::VectorWidget(XMVECTOR* vector, QWidget* parent)
{
	_vector = vector;

	xSpinbox = new QDoubleSpinBox();
	ySpinbox = new QDoubleSpinBox();
	zSpinbox = new QDoubleSpinBox();

	connect(xSpinbox, &QDoubleSpinBox::editingFinished, this, &VectorWidget::SetValue);
	connect(ySpinbox, &QDoubleSpinBox::editingFinished, this, &VectorWidget::SetValue);
	connect(zSpinbox, &QDoubleSpinBox::editingFinished, this, &VectorWidget::SetValue);

	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(xSpinbox);
	hLayout->addWidget(ySpinbox);
	hLayout->addWidget(zSpinbox);

	setLayout(hLayout);
}

void VectorWidget::SetValue()
{
	XMVECTOR newVector = XMVectorSet((float)xSpinbox->value(), (float)ySpinbox->value(), (float)zSpinbox->value(), 0.f);
	*_vector = newVector;
}

void VectorWidget::ResetValue()
{
	xSpinbox->setValue(_vector->m128_f32[0]);
	ySpinbox->setValue(_vector->m128_f32[1]);
	zSpinbox->setValue(_vector->m128_f32[2]);
}
