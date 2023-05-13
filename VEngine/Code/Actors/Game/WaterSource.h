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
	Properties GetProps() override;

	void Activate() override;
	void Deactivate() override;

	bool Contains(XMVECTOR point);

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
