#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;
struct InteractWidget;

struct EntranceTrigger : Actor
{
	ACTOR_SYSTEM(EntranceTrigger)

	BoxTriggerComponent* trigger = nullptr;

	InteractWidget* interactWidget = nullptr;

	std::string levelToMoveTo;
	std::string openText;

	bool isEntraceActive = true;

	//Whether the player has used the entrance to move between worlds.
	bool entranceInteractedWith = false;

	EntranceTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
