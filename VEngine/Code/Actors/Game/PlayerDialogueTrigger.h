#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//Actor to trigger quick player dialogue when player overlaps with trigger.
class PlayerDialogueTrigger : public Actor
{
public:
	ACTOR_SYSTEM(PlayerDialogueTrigger);

	PlayerDialogueTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	BoxTriggerComponent* trigger = nullptr;

	//Text to show on overlap
	std::wstring playerThoughtText;

	bool alreadyActivated = false;
};
