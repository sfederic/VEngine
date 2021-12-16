#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;
struct DialogueComponent;

//Triggers a dialogue when player is inside this trigger
struct DialogueTrigger : Actor
{
	ACTOR_SYSTEM(DialogueTrigger);

	BoxTriggerComponent* trigger = nullptr;
	DialogueComponent* dialogueComponent = nullptr;

	std::string dialogueFile;

	bool walkInToActivate = false;

	DialogueTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
