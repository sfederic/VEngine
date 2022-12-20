#pragma once

#include "Enemy.h"

//Enemy with a hidden core at its center, moving to obscure the player hitting it.
class CoreEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(CoreEnemy);

	virtual void Create() override;
	virtual Properties GetProps() override;
};
