#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//This will just be a BoxTrigger that will "use up" the player's held item.
//Might also be that it spawns said held item for visual reference in the world.
class PickupPlacementActor : public Actor
{
public:
	ACTOR_SYSTEM(PickupPlacementActor);

	PickupPlacementActor();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	std::string actorToActivateName;
	Actor* actorToActivate = nullptr;
};
