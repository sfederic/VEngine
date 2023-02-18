#pragma once

#include "Enemy.h"

class MeshComponent;
struct BoxTriggerComponent;

//Enemy that teleports around the map.
class TeleportingEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(TeleportingEnemy);

	TeleportingEnemy();
	void Start() override;
	void Create() override;

private:
	void Teleport();

	BoxTriggerComponent* boxTrigger = nullptr;
	MeshComponent* mesh = nullptr;
};
