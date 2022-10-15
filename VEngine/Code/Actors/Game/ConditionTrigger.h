#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct ConditionComponent;
struct BoxTriggerComponent;

//Trigger that sets off a condition in the world.
//Used as a box trigger or as an in world reference.
class ConditionTrigger : public Actor
{
public:
	ACTOR_SYSTEM(ConditionTrigger);

	ConditionTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	ConditionComponent* condition = nullptr;

	BoxTriggerComponent* boxTrigger = nullptr;
};
