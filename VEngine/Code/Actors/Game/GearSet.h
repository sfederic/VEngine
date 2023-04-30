#pragma once

#include "GridActor.h"

class BoxTriggerComponent;
class MeshComponent;

class GearSet : public GridActor
{
public:
	ACTOR_SYSTEM(GearSet);

	GearSet();
	void Create() override;
	Properties GetProps() override;

	bool Intersects(const BoundingOrientedBox& intersectCheck) const;

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
