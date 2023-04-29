#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;

class GearSet : public Actor
{
public:
	ACTOR_SYSTEM(GearSet);

	GearSet();
	void Create() override;
	Properties GetProps() override;

	bool Intersects(const BoundingOrientedBox& intersectCheck) const;

private:
	BoxTriggerComponent* boxTrigger = nullptr;
	MeshComponent* mesh = nullptr;
};
