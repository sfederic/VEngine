#pragma once

#include "GridActor.h"

//Power plate/slot that powers up a linked actor when a power crystal is on this actor.
class PowerCrystalSlot : public GridActor
{
public:
	ACTOR_SYSTEM(PowerCrystalSlot);

	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	std::string powerUpActorName;
	GridActor* powerUpActor = nullptr;
};
