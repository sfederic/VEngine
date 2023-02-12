#pragma once

#include "Enemy.h"

class MeshComponent;
class MeshSplitActor;

//Enemy that can't be hit unless player severs power supply cables to it.
class PowerCableEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(PowerCableEnemy);

	PowerCableEnemy();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};
