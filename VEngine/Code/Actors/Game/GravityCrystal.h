#pragma once

#include "GridActor.h"

class BoxTriggerComponent;
class MeshComponent;

class GravityCrystal : public GridActor
{
public:
	ACTOR_SYSTEM(GravityCrystal);

	GravityCrystal();
	void Create() override;
	void Tick(float deltaTime);

private:
	std::set<GridActor*> previouslyContainedActors;
	BoxTriggerComponent* gravityInfluenceTrigger = nullptr;
	MeshComponent* gravityFieldMesh = nullptr;
};
