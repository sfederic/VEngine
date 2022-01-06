#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct DialogueComponent;

//Conversations are just dialog files that use NPCs to talk with themselves with or without player input.
//eg. walking into room, walking into a Conversation's trigger.
struct Conversation : Actor
{
	ACTOR_SYSTEM(Conversation);

	DialogueComponent* dialogueComponent = nullptr;

	bool playOnSpawn = true;

	Conversation();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void NextLine();
};
