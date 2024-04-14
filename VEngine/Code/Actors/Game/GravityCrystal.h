#pragma once

#include "GridActor.h"

class GravityCrystal : public GridActor
{
public:
	ACTOR_SYSTEM(GravityCrystal);

	GravityCrystal();
	void Create() override;
	void Tick(float deltaTime);

private:
	class BoxTriggerComponent* gravityInfluenceTrigger = nullptr;

	std::set<GridActor*> previouslyContainedActors;
};
