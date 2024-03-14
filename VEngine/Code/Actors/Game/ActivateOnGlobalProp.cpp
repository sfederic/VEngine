#include "vpch.h"
#include "ActivateOnGlobalProp.h"
#include "Gameplay/GameInstance.h"
#include "Core/PropertyTypes.h"

ActivateOnGlobalProp::ActivateOnGlobalProp()
{
	propertyType = PropertyTypes::GetPropertyTypes();
}

void ActivateOnGlobalProp::Start()
{
	__super::Start();

	if (propertyName.empty())
	{
		Log("No property name [%s] set for [%s].", propertyName.c_str(), GetName().c_str());
		return;
	}

	if (propertyType.Compare(PropertyTypes::Bool))
	{
		bool* value = GameInstance::GetGlobalProp<bool>(propertyName);
		if (value != nullptr)
		{
			if (*value == true)
			{
				Activate();
			}
		}
	}
}

Properties ActivateOnGlobalProp::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Property Name", &propertyName);
	props.Add("Property Type", &propertyType);
	return props;
}
