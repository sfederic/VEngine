#pragma once

#include "Enemy.h"

//Sits in one spot and shoots forward.
class TurretEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(TurretEnemy);

	void Create() override;
	Properties GetProps() override;

private:
	void Shoot();
};
