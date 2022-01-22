#include "VEnumWidget.h"
#include "VEnum.h"

VEnumWidget::VEnumWidget(Property value_)
{
	prop = value_;
	value = (VEnum*)value_.data;

	for (auto& name : value->GetAllNames())
	{
		addItem(QString::fromStdString(name));
	}

	connect(this, SIGNAL(QComboBox::currentIndexChanged(int)), this, SLOT(VEnumWidget::SetValue));
}

void VEnumWidget::SetValue(int index)
{
	setCurrentIndex(index);
	std::string text = currentText().toStdString();
	value->SetValue(text);
}

void VEnumWidget::ResetValue()
{
}
