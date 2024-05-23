#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//Put a BloomSeed in this actor's trigger to plant it.
class BloomSeedSoil : public GridActor
{
public:
	ACTOR_SYSTEM(BloomSeedSoil);
	BloomSeedSoil();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
