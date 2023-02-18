#pragma once
#include <qcombobox.h>
#include "IPropertyWidget.h"

class VEnum;
class QWheelEvent;

struct VEnumWidget : public QComboBox, IPropertyWidget
{
	VEnum* value = nullptr;

	VEnumWidget(Property& value_);
	void SetValue();
	virtual void ResetValue() override;

	void wheelEvent(QWheelEvent* event_) override;
};
