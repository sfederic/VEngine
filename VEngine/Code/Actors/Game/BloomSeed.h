#pragma once

#include "GridActor.h"

//@Todo: actors needs a visual overhaul.

//See that blooms into a flower platform when planted.
class BloomSeed : public GridActor
{
public:
	ACTOR_SYSTEM(BloomSeed);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void Test();

	bool mIsPlanted = false;
};
