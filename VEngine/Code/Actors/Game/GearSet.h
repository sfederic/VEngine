#pragma once

#include "GridActor.h"

class BoxTriggerComponent;
class MeshComponent;
namespace DirectX {
	struct BoundingOrientedBox;
}

class GearSet : public GridActor
{
public:
	ACTOR_SYSTEM(GearSet);

	GearSet();
	void Create() override;
	Properties GetProps() override;

	void OnLinkRotate() override;
	void PowerUp() override;

	bool Intersects(const DirectX::BoundingOrientedBox& intersectCheck) const;
	void IncrementDoor();

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	std::string gearSetDoorName;
	int numDoorIncrementsDone = 0;
};
