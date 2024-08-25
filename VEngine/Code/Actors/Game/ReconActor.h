#pragma once

#include "GridActor.h"

//An Actor the player can recon while interacting in recon mode.
class ReconActor : public GridActor
{
public:
	ACTOR_SYSTEM(ReconActor);
};
