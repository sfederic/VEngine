#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;

//Swaps the positions of two grid actors after two of these teleporters have been set up.
class GridActorTeleporter : public Actor
{
public:
	ACTOR_SYSTEM(GridActorTeleporter);

	GridActorTeleporter();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	std::string _otherTeleporterName;
	//Note that only one teleporter needs to have this setup.
	GridActorTeleporter* _otherTeleporter = nullptr;

	MeshComponent* _teleporterMesh = nullptr;

public:
	BoxTriggerComponent* boxTrigger = nullptr;

private:
	//This is used as a 'cooldown' bool so that both grid actors aren't teleporting non-stop between teleporters.
	bool _teleporterUsed = false;
};
