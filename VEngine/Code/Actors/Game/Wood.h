#pragma once

#include "GridActor.h"

//To be chopped/collected and placed into a kiln.
class Wood : public GridActor
{
public:
	ACTOR_SYSTEM(Wood);
	
	void Create() override;
};
