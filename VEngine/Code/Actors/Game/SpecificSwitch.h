#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;
class BoxTriggerComponent;
class GridActor;

//Node that looks for a specific grid actor to be moved onto it to activate.
class SpecificSwitch : public Actor
{
public:
	ACTOR_SYSTEM(SpecificSwitch);

	SpecificSwitch();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	bool IsSwitchActivated() { return isActivated; }

private:
	void CheckForSpecificGridActorOverlapping();

	MeshComponent* mesh = nullptr;
	BoxTriggerComponent* trigger = nullptr;

	GridActor* gridActorToCheck = nullptr;
	std::string gridActorToCheckName;

	bool isActivated = false;
};
