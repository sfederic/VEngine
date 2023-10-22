#pragma once

#include <qcombobox.h>
#include "IPropertyWidget.h"

class VEnum;
class QWheelEvent;

class VEnumWidget : public QComboBox, public IPropertyWidget
{
private:
	VEnum* value = nullptr;

public:
	VEnumWidget(Property& value_);
	void SetValue();
	virtual void ResetValue() override;

protected:
	void wheelEvent(QWheelEvent* event_) override;
};
