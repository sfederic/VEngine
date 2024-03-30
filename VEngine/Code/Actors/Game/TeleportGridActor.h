#pragma once

#include "GridActor.h"

//Actor that 'teleports' to the furthest node it can on the axis it's moved. Think like when you
//do a quick place in Tetris.
// 
//The code here is only going to work with grid aligned actors. If you do use this actor,
//make sure the actors in the world are cubic.
class TeleportGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(TeleportGridActor);

	void OnLinkMove() override;
};
