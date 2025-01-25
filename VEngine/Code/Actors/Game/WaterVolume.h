#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;
class GridActor;

class WaterVolume : public Actor
{
public:
	ACTOR_SYSTEM(WaterVolume);

	WaterVolume();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	bool Contains(DirectX::XMVECTOR point);
	std::vector<GridActor*> GetAllGridActorsWithinVolume();

private:
	void DouseGridActorsInWaterVolume();

	MeshComponent* waterMesh = nullptr;
	BoxTriggerComponent* waterVolumeTrigger = nullptr;
};
