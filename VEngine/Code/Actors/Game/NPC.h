#pragma once
#include "Unit.h"
#include "../ActorSystem.h"

struct DialogueComponent;

//Actor that can be talked to and interactied with.
struct NPC : Unit
{
	ACTOR_SYSTEM(NPC)

	DialogueComponent* dialogueComponent = nullptr;

	bool isQuickDialogueActive = false;

public:
	NPC();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	//a once off dialogue popup used like interact popup is.
	void QuickTalkTo();
	void EndQuickTalkTo();
};
