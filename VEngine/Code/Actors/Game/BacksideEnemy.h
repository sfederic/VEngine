#pragma once

#include "Enemy.h"

//Enemy the player has to attack at its back by comparing player's forward vector.
class BacksideEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(BacksideEnemy);

	void Create() override;
	bool CanBeHit(AttackTypes attackType) override;
};
