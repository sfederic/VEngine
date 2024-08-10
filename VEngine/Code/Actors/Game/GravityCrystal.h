#pragma once

#include "GridActor.h"

class BoxTriggerComponent;
class MeshComponent;

//Todo: This is a bit of a weird grid actor. Potential for a lot of in-game bugs, think about deleting it.
class GravityCrystal : public GridActor
{
public:
	ACTOR_SYSTEM(GravityCrystal);

	GravityCrystal();
	void Create() override;
	void Tick(float deltaTime);
	Properties GetProps() override;

private:
	std::set<GridActor*> previouslyContainedActors;
	BoxTriggerComponent* gravityInfluenceTrigger = nullptr;
	MeshComponent* gravityFieldMesh = nullptr;
};
