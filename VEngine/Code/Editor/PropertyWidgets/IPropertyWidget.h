#pragma once
#include "Property.h"

struct IPropertyWidget
{
	Property prop;
	virtual void ResetValue() = 0;
};
