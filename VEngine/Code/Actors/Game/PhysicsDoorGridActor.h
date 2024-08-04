#pragma once

#include "GridActor.h"

//Entrance like a door, vent or grate that is pulled off its hinges when moved when linked to,
//causing it to turn on physics to serve as a little animation when hitting the floor.
class PhysicsDoorGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(PhysicsDoorGridActor);

	void Create() override;
	Properties GetProps() override;

	void OnLinkMove() override;

private:
	bool hasBeenUnhinged = false;
};
