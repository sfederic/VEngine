#pragma once

#include "Enemy.h"

//Enemy that the player needs to orient itself with propely to destroy on bladeswipe.
class OrientationEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(OrientationEnemy);

	void Create() override;
	bool CanBeHit(AttackTypes attackType) override;
};
