#pragma once
#include "GridActor.h"
#include "../ActorSystem.h"

struct DialogueComponent;
struct WidgetComponent;

struct NPC : GridActor
{
	ACTOR_SYSTEM(NPC)

	DialogueComponent* dialogue = nullptr;
	WidgetComponent* dialogueWidget = nullptr;

	NPC();
	virtual void Start() override;
	virtual Properties GetProps() override;
	void TalkTo();
};
