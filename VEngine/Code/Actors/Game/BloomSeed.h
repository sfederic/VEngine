#pragma once

#include "GridActor.h"

//@Todo: actors needs a visual overhaul.

//See that blooms into a flower platform when planted.
class BloomSeed : public GridActor
{
public:
	ACTOR_SYSTEM(BloomSeed);

	void Create() override;
	Properties GetProps() override;

	void Plant();

private:
	bool mIsPlanted = false;
};
