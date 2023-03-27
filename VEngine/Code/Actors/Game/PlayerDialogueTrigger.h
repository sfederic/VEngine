#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;

//Actor to trigger quick player dialogue when player overlaps with trigger.
struct PlayerDialogueTrigger : Actor
{
	ACTOR_SYSTEM(PlayerDialogueTrigger);

	BoxTriggerComponent* trigger = nullptr;

	//Text to show on overlap
	std::wstring playerThoughtText;

	//Memory that can optionally activate player dialogue
	std::string memoryToActivate;

	//@Todo: wil need to come back here and fiddle with this bool in props for game saves.
	bool alreadyActivated = false;

	PlayerDialogueTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
