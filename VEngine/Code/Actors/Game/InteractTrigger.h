#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"
#include <string>

struct BoxTriggerComponent;
struct Widget;
struct InteractWidget;

//Used to open up a quick prompt/action when the player overlaps
struct InteractTrigger : Actor
{
	ACTOR_SYSTEM(InteractTrigger)

	BoxTriggerComponent* trigger = nullptr;

	InteractWidget* interactWidget = nullptr;

	Actor* targetActor = nullptr;

	std::string interactText;

	InteractTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
