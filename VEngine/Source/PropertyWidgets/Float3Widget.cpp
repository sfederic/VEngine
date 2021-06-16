#include "Float3Widget.h"
#include <qboxlayout.h>

Float3Widget::Float3Widget(XMFLOAT3* value, QWidget* parent)
{
	_value = value;

	setDecimals(4);
	setMinimum(std::numeric_limits<float>::lowest());
	setMaximum(std::numeric_limits<double>::max());
	setButtonSymbols(QAbstractSpinBox::NoButtons);

	connect(&xSpinbox, &QDoubleSpinBox::editingFinished, this, &Float3Widget::SetValue);
	connect(&ySpinbox, &QDoubleSpinBox::editingFinished, this, &Float3Widget::SetValue);
	connect(&zSpinbox, &QDoubleSpinBox::editingFinished, this, &Float3Widget::SetValue);

	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(&xSpinbox);
	hLayout->addWidget(&ySpinbox);
	hLayout->addWidget(&zSpinbox);

	setLayout(hLayout);

	connect(this, &QDoubleSpinBox::editingFinished, this, &Float3Widget::SetValue);
}

void Float3Widget::SetValue()
{
	XMFLOAT3 newFloat3 = {};
	newFloat3.x = xSpinbox.value();
	newFloat3.y = ySpinbox.value();
	newFloat3.z = zSpinbox.value();
	*_value = newFloat3;
}
