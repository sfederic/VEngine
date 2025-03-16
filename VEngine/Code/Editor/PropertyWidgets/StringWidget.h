#pragma once

import <string>;
#include <qlineedit.h>
#include "IPropertyWidget.h"

class StringWidget : public QLineEdit, public IPropertyWidget
{
public:
	StringWidget(Property& value_);
	void SetValue();
	void ResetValue() override;

private:
	void SetDirectoryFilenamesAutoComplete(const std::string& directoryPath);
	void SetActorWorldListAutoComplete();

	std::string* value;
};
