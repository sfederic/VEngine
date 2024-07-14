#include "vpch.h"
#include "VEnumWidget.h"
#include <QWheelEvent>
#include "Core/VEnum.h"

VEnumWidget::VEnumWidget(Property& value_)
{
	prop = value_;
	value = value_.GetData<VEnum>();

	for (auto& name : value->GetAllNames())
	{
		addItem(QString::fromStdString(name));
	}

	setCurrentText(QString::fromStdString(value->GetValue()));
	setFocusPolicy(Qt::StrongFocus);
	connect(this, &QComboBox::currentTextChanged, this, &VEnumWidget::SetValue);
}

void VEnumWidget::SetValue()
{
	const std::string text = currentText().toStdString();

	if (!value->Compare(text))
	{
		IPropertyWidget::SetValue<VEnum>(prop);
	}

	value->SetValue(text);

	if (prop.change)
	{
		prop.change(prop);
	}

	clearFocus();
}

void VEnumWidget::ResetValue()
{
}

void VEnumWidget::wheelEvent(QWheelEvent* event_)
{
	!hasFocus() ? event_->ignore() : QComboBox::wheelEvent(event_);
}
