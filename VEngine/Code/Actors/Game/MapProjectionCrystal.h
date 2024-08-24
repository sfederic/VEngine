#pragma once

#include "GridActor.h"

//A projection crystal that turns on all actor projections in the map.
//Think sort of like how the Time Stones work in Skyward Sword.
class MapProjectionCrystal : public GridActor
{
public:
	ACTOR_SYSTEM(MapProjectionCrystal);

	void Interact() override;
};
