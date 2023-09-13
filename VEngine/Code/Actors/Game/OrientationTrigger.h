#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//Same as an OrientationLock, except this trigger expects an actor within it alongside the correct orientation.
class OrientationTrigger : public Actor
{
public:
	ACTOR_SYSTEM(OrientationTrigger);

	OrientationTrigger();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void CheckIfOrientationLockIsAlignedInTrigger();

	BoxTriggerComponent* boxTrigger = nullptr;

	bool actorInTrigger = false;
};
