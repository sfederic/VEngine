#pragma once

#include "Enemy.h"

//Enemy that the player needs to orient itself with propely to destroy with blade.
class OrientationEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(OrientationEnemy);

	void Create() override;
	void Tick(float deltaTime) override;
	bool CanBeHit(AttackTypes attackType) override;
};
