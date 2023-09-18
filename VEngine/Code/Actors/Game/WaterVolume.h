#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;
class GridActor;

//A plane denoting the water's surface and a BoxTrigger encapsulating the body of water.
class WaterVolume : public Actor
{
public:
	ACTOR_SYSTEM(WaterVolume);

	WaterVolume();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	bool Contains(XMVECTOR point);
	std::vector<GridActor*> GetAllGridActorsWithinVolume();

	float GetYPointToRaiseTo() { return yPointToRaiseTo; }
	void SetYPointToRaiseTo(float value) { yPointToRaiseTo = value; }
	void IncreaseYPointToRaiseTo(float value) { yPointToRaiseTo += value; }

private:
	void DouseGridActorsInWaterVolume();

	MeshComponent* waterSurface = nullptr;
	BoxTriggerComponent* waterVolumeTrigger = nullptr;

	float yPointToRaiseTo = 0.f;
};
