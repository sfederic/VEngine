#pragma once

#include "GridActor.h"

//Actor that 'teleports' to the furthest node it can on the axis it's moved. Think like when you
//do a quick place in Tetris.
class TeleportGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(TeleportGridActor);

	void OnLinkMove() override;
};
