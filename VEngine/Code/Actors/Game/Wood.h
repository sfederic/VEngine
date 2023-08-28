#pragma once

#include "Pickup.h"

//To be chopped/collected and placed into a kiln.
class Wood : public Pickup
{
public:
	ACTOR_SYSTEM(Wood);

	void Create() override;
	void Slice() override;
	void Interact() override;

private:
	bool hasBeenCut = false;
};
