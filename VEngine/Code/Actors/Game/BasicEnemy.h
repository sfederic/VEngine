#pragma once

#include "Enemy.h"

//Enemy that just spins around, not moving.
class BasicEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(BasicEnemy);

	BasicEnemy();
	void Tick(float deltaTime) override;
};
