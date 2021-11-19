#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct DialogueComponent;

struct DialogueInstance : Actor
{
	ACTOR_SYSTEM(DialogueInstance)

	DialogueComponent* dialogueComponent = nullptr;

	DialogueInstance();
	virtual Properties GetProps() override;
};
