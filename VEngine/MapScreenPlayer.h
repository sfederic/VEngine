#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"

class CameraComponent;

class MapScreenPlayer : public Actor
{
public:
	ACTOR_SYSTEM(MapScreenPlayer);

	MapScreenPlayer();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	void HandleInput(float deltaTime);

	CameraComponent* camera = nullptr;
};
