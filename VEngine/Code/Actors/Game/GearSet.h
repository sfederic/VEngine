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
	Properties GetProps() override;

	bool Contains(XMVECTOR point);

private:
	BoxTriggerComponent* boxTrigger = nullptr;
	MeshComponent* mesh = nullptr;
};
