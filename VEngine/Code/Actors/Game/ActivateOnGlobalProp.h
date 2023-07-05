#pragma once

#include "GridActor.h"
#include "Core/VEnum.h"

//Grid actor that activates when a specified global property exists or has the correct value.
class ActivateOnGlobalProp : public GridActor
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
