#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class OrientationTrigger;

class MultiOrientationTriggerLock : public Actor
{
public:
	ACTOR_SYSTEM(MultiOrientationTriggerLock);

	MultiOrientationTriggerLock();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void CheckIfAllOrientationTriggersAreCorrect();

	std::string triggerName0;
	std::string triggerName1;
	std::string triggerName2;
	std::string triggerName3;

	std::vector<OrientationTrigger*> triggerActors;

	std::string actorToActivate;
	Actor* actorToActivateOnAllLocksBeingCorrect = nullptr;

	bool onceOffAudioCueOnActivate = false;
};
