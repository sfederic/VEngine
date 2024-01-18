#pragma once

#include "GridActor.h"

//This is a specific once off GridActor for the Town's Clock Tower that sets a global prop for the town's 
//time of day based on this actor's rotation.
class TimeOfDayWheel : public GridActor
{
public:
	ACTOR_SYSTEM(TimeOfDayWheel);

	void OnRotationEnd() override;
	Properties GetProps() override;
};
