#pragma once

#include "Core/Property.h"

class IPropertyWidget
{
public:
	Property prop;
	virtual void ResetValue() = 0;
};
