#include "vpch.h"
#include "PropertyTypes.h"

static VEnum propertyTypesEnum;

void PropertyTypes::SetupPropertyTypesVEnum()
{
	propertyTypesEnum.Reset();

	propertyTypesEnum.Add(PropertyTypes::Bool);
	propertyTypesEnum.Add(PropertyTypes::Int);
	propertyTypesEnum.Add(PropertyTypes::Float);
	propertyTypesEnum.Add(PropertyTypes::String);
}

VEnum PropertyTypes::GetPropertyTypes()
{
	return propertyTypesEnum;
}
