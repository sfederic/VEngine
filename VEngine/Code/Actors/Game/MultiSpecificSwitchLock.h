#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class SpecificSwitch;

//Checks if multiple SpecificSwitchs are active to activate another designed actor.
class MultiSpecificSwitchLock : public Actor
{
public:
	ACTOR_SYSTEM(MultiSpecificSwitchLock);

	MultiSpecificSwitchLock();

	void Start();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void CheckIfAllActorOrientationsAreCorrect();

	std::string switchActorName0;
	std::string switchActorName1;
	std::string switchActorName2;
	std::string switchActorName3;

	std::vector<SpecificSwitch*> switchActors;

	std::string actorToActivateName;
	Actor* actorToActivateOnAllSwitchesActive = nullptr;

	bool onceOffAudioCueOnActivate = false;
};
