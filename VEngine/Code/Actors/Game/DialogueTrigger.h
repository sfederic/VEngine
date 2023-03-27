#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct DialogueComponent;
struct BoxTriggerComponent;

//Conversations are just dialog files that use NPCs to talk with themselves with or without player input.
//eg. walking into room, walking into a Conversation's trigger.
struct DialogueTrigger : Actor
{
	ACTOR_SYSTEM(DialogueTrigger);

	DialogueComponent* dialogueComponent = nullptr;
	BoxTriggerComponent* boxTriggerComponent = nullptr;

private:
	bool playOnSpawn = false;

	bool playOnTriggerOverlap = false;
	bool firstTimePlaying = true;

	bool dialogueFinished = false;

public:
	DialogueTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void NextLine();
};
