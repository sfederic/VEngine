#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;

//In-world save point the player can interact with to save.
struct SavePoint : Actor
{
	ACTOR_SYSTEM(SavePoint);

	BoxTriggerComponent* trigger = nullptr;

	SavePoint();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
