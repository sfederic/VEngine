export module Actors.Game.ActivateOnGlobalProp;

import Core.Properties;
import Core.PropertyTypes;
import Core.VEnum;
import Core.Log;
import <string>;

#include "Actors/ActorSystemDefine.h"

//Grid actor that activates when a specified global property exists or has the correct value.
export class ActivateOnGlobalProp : public GridActor
{
public:
	ACTOR_SYSTEM(ActivateOnGlobalProp);

	ActivateOnGlobalProp();
	void Start() override;
	Properties GetProps() override;

private:
	std::string propertyName;
	VEnum propertyType;
};

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
