#include "vpch.h"
#include "VectorWidget.h"
#include <QHBoxLayout>
#include <QDoubleSpinBox>

VectorWidget::VectorWidget(Property& prop_, QWidget* parent)
{
	prop = prop_;
	_vector = prop.GetData<XMVECTOR>();

	xSpinbox = new QDoubleSpinBox();
	ySpinbox = new QDoubleSpinBox();
	zSpinbox = new QDoubleSpinBox();
	wSpinbox = new QDoubleSpinBox();

	connect(xSpinbox, &QDoubleSpinBox::editingFinished, this, &VectorWidget::SetValue);
	connect(ySpinbox, &QDoubleSpinBox::editingFinished, this, &VectorWidget::SetValue);
	connect(zSpinbox, &QDoubleSpinBox::editingFinished, this, &VectorWidget::SetValue);
	connect(wSpinbox, &QDoubleSpinBox::editingFinished, this, &VectorWidget::SetValue);

	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(xSpinbox);
	hLayout->addWidget(ySpinbox);
	hLayout->addWidget(zSpinbox);
	hLayout->addWidget(wSpinbox);

	setLayout(hLayout);
}

void VectorWidget::SetValue()
{
	IPropertyWidget::SetValue<XMVECTOR>(prop);

	XMVECTOR newVector = XMVectorSet(static_cast<float>(xSpinbox->value()),
		static_cast<float>(ySpinbox->value()),
		static_cast<float>(zSpinbox->value()),
		static_cast<float>(wSpinbox->value()));
	*_vector = newVector;

	IPropertyWidget::SetValue<XMVECTOR>(prop);
}

void VectorWidget::ResetValue()
{
	xSpinbox->setValue(_vector->m128_f32[0]);
	ySpinbox->setValue(_vector->m128_f32[1]);
	zSpinbox->setValue(_vector->m128_f32[2]);
	wSpinbox->setValue(_vector->m128_f32[3]);
}
