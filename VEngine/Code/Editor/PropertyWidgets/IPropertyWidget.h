#pragma once

#include "Property.h"

class IPropertyWidget
{
public:
	Property prop;
	virtual void ResetValue() = 0;
};
