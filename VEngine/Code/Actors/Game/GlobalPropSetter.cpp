
#include "GlobalPropSetter.h"
#include "Gameplay/GameInstance.h"
#include "Core/Log.h"

GlobalPropSetter::GlobalPropSetter()
{
	SetEmptyRootComponent();
}

void GlobalPropSetter::Activate()
{
	if (hasBeenActivated)
	{
		return;
	}

	hasBeenActivated = true;

	__super::Activate();

	bool* value = GameInstance::GetGlobalProp<bool>(propertyName);
	if (value != nullptr)
	{
		*value = propertyValue;
		Log("[%s] set global property [%s] to [%d]", GetName().c_str(), propertyName.c_str(), value);
	}
}

void GlobalPropSetter::Deactivate()
{
	if (!hasBeenActivated)
	{
		return;
	}

	hasBeenActivated = false;

	__super::Deactivate();

	bool* value = GameInstance::GetGlobalProp<bool>(propertyName);
	if (value != nullptr)
	{
		*value = !(*value);
		Log("[%s] set global property [%s] to [%d]", GetName().c_str(), propertyName.c_str(), value);
	}
}

Properties GlobalPropSetter::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Property Name", &propertyName);
	props.Add("Property Value", &propertyValue);
	props.Add("Activated", &hasBeenActivated).readOnly = true;
	return props;
}
