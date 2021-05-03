#pragma once

#include <qcheckbox.h>

class BoolWidget : public QCheckBox
{
public:
	BoolWidget(bool* value, const char* name, QWidget* parent = 0);

private:
	void SetValue(int value);

	bool* _value;
};
