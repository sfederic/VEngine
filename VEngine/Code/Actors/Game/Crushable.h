#pragma once

#include "GridActor.h"

//Something a weight can destroy when on top of it or drilled.
class Crushable : public GridActor
{
public:
	ACTOR_SYSTEM(Crushable);

	void Crushed();
	void Attacked() override;
};
