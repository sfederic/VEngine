#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;

class WaterSource : public Actor
{
public:
	ACTOR_SYSTEM(WaterSource);

	WaterSource();
	void Create() override;

	bool Contains(XMVECTOR point);

private:
	BoxTriggerComponent* boxTrigger = nullptr;
	MeshComponent* mesh = nullptr;
};
