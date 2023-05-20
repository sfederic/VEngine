#pragma once

#include "GridActor.h"

//@Todo: player needs to not be able to walk on this. Maybe NPCs too.

//Node grid actor that needs to be doused to step onto.
class HotCoals : public GridActor
{
public:
	ACTOR_SYSTEM(HotCoals);

	void Create() override;

	void Douse() override;
};
