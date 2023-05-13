#pragma once

#include "GridActor.h"

//Something a weight can destroy when on top of it
class Crushable : public GridActor
{
public:
	ACTOR_SYSTEM(Crushable);

	void Crushed();
};
