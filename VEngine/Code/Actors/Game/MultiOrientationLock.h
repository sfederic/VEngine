#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class OrientationLock;
class GridActor;

//Takes the names of multiple OrientatonLock actors and activates this actor if all their orientations are correct.
//Note that this actor isn't a Grid Actor. It's more like an inanimate manager handling Orientation Locks.
class MultiOrientationLock : public Actor
{
public:
	ACTOR_SYSTEM(MultiOrientationLock);

	MultiOrientationLock();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void CheckIfAllActorOrientationsAreCorrect();

	std::string lockActorName0;
	std::string lockActorName1;
	std::string lockActorName2;
	std::string lockActorName3;

	std::vector<OrientationLock*> lockActors;

	std::string actorToActivate;
	Actor* actorToActivateOnAllLocksBeingCorrect = nullptr;

	bool onceOffAudioCueOnActivate = false;
};
