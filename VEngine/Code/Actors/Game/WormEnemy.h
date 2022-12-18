#pragma once

#include "Enemy.h"

class WormEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(WormEnemy);

	WormEnemy();
	virtual Properties GetProps() override;
};
