#pragma once

#include "Enemy.h"

//Enemy with multiple meshes joined up.
class WormEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(WormEnemy);

	WormEnemy();
	virtual Properties GetProps() override;
};
