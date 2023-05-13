#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;

//A plane denoting the water's surface and a BoxTrigger encapsulating the body of water.
class WaterVolume : public Actor
{
public:
	ACTOR_SYSTEM(WaterVolume);

	WaterVolume();
	void Create() override;
	Properties GetProps() override;

private:
	MeshComponent* waterSurface = nullptr;
	BoxTriggerComponent* waterVolumeTrigger = nullptr;
};
