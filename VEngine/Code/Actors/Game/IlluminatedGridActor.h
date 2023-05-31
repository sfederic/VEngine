#pragma once

#include "GridActor.h"

//Actor that can be 'illuminated' by light sources.
class IlluminatedGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(IlluminatedGridActor);

	void Illuminate();
};
