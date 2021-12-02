#pragma once
#include "GridActor.h"
#include "../ActorSystem.h"

struct DialogueComponent;
struct WidgetComponent;

struct NPC : GridActor
{
	ACTOR_SYSTEM(NPC)

	DialogueComponent* dialogueComponent = nullptr;

	NPC();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	//a once off dialogue popup used like interact popup is.
	void QuickTalkTo();
};
