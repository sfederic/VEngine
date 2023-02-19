#pragma once

#include "Enemy.h"

class MeshComponent;
class BoxTriggerComponent;

//Enemy that teleports around the map.
class TeleportingEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(TeleportingEnemy);

	TeleportingEnemy();
	void Tick(float deltaTime) override;
	void Create() override;

private:
	void Teleport();

	BoxTriggerComponent* boxTrigger = nullptr;
	MeshComponent* mesh = nullptr;

	float teleportTimer = 0.f;
};
