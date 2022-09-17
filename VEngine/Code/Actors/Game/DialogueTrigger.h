#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;

class DialogueTrigger : public Actor
{
public:
	ACTOR_SYSTEM(DialogueTrigger);

	DialogueTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	BoxTriggerComponent* boxTriggerComponent = nullptr;

	std::string dialogueFile;

	bool dialogueCurrentlyPlaying = false;
};

