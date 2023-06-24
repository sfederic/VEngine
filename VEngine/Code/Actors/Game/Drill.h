#pragma once

#include "GridActor.h"

class Drill : public GridActor
{
public:
	ACTOR_SYSTEM(Drill);

	void Create() override;

	void OnLinkMove() override;
};
