#pragma once

#include "GridActor.h"

//Works with LookingGlass to be revealed once a raycast goes from this actor,
//through a LookingGlass, to the camera.
class InvisibleGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(InvisibleGridActor);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;
};
