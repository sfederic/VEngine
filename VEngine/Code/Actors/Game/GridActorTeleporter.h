#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//Swaps the positions of two grid actors after two of these teleporters have been set up.
class GridActorTeleporter : public Actor
{
public:
	ACTOR_SYSTEM(GridActorTeleporter);

	GridActorTeleporter();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	std::string _otherTeleporterName;
	//Note that only one teleporter needs to have this setup.
	GridActorTeleporter* _otherTeleporter = nullptr;

public:
	BoxTriggerComponent* boxTrigger = nullptr;

private:
	//This will trigger when both grid actors are in both teleporters.
	bool _teleporterUsed = false;
};
