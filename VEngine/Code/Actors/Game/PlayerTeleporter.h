#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

class PlayerTeleporter : public Actor
{
public:
	ACTOR_SYSTEM(PlayerTeleporter);

	PlayerTeleporter();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	std::string linkedTeleporterName;
	PlayerTeleporter* linkedTeleporter = nullptr;

	BoxTriggerComponent* boxTrigger = nullptr;

public:
	bool hasBeenUsed = false;
};
