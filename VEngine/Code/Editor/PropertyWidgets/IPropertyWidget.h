#pragma once
#include "Properties.h"

struct IPropertyWidget
{
	Property prop;
	virtual void ResetValue() = 0;
};
