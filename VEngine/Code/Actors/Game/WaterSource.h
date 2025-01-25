#pragma once

#include "GridActor.h"

class BoxTriggerComponent;
class MeshComponent;

class WaterSource : public GridActor
{
public:
	ACTOR_SYSTEM(WaterSource);

	WaterSource();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void Activate() override;
	void Deactivate() override;

	bool Contains(DirectX::XMVECTOR point);

private:
	void DouseGridActorsInTrigger();

	BoxTriggerComponent* boxTrigger = nullptr;
};
