#pragma once

#include "FenceActor.h"

//A door or entrance that works like a regular door on hinges. Is meant to mechanically highlight
//rotating doors on an offset, so that the player is not just moving the door itself around.
class PivotDoor : public FenceActor
{
public:
	ACTOR_SYSTEM(PivotDoor);

	PivotDoor();
	void Create() override;
	Properties GetProps() override;
};
