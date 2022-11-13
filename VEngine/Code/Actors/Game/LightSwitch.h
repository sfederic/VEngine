#pragma once

#include "GridActor.h"

struct PointLightComponent;

//Point Light switch that turns on and off when it's attacked.
class LightSwitch : public GridActor
{
public:
	ACTOR_SYSTEM(LightSwitch);

	LightSwitch();
	virtual Properties GetProps() override;

	virtual void Interact() override;

private:
	PointLightComponent* pointLight = nullptr;
};
