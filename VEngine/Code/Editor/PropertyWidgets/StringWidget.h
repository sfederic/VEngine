#pragma once

#include <string>
#include <qlineedit.h>
#include "IPropertyWidget.h"

struct StringWidget : public QLineEdit, IPropertyWidget
{
	std::string* value;

	StringWidget(Property& value_);
	void SetValue();
	virtual void ResetValue() override;

private:
	void SetDirectoryFilenamesAutoComplete(const std::string& directoryPath);
	void SetActorWorldListAutoComplete();
};
