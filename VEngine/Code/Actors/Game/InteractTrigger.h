#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"
#include <string>

struct BoxTriggerComponent;
struct IntuitionComponent;
struct Widget;
struct InteractWidget;

//Used to open up a quick prompt/action when the player overlaps
struct InteractTrigger : Actor
{
	ACTOR_SYSTEM(InteractTrigger)

	BoxTriggerComponent* trigger = nullptr;

	IntuitionComponent* intuitionComponent = nullptr;

	//Widget shown on interact
	InteractWidget* interactWidget = nullptr;

	//text to show on PrimaryAction() player input
	std::string interactText;

	//text to show when intuition condition has passed as true
	std::string interactKnown;

	//Text to show when player is inside the trigger
	std::string overlapText = "Examine";

	std::string targetActorName;

	bool isBeingInteractedWith = false;

	InteractTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
