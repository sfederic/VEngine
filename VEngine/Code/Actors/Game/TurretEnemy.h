#pragma once

#include "Enemy.h"

//Sits in one spot and shoots forward.
class TurretEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(TurretEnemy);

	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void Shoot();

	float shootTimer = 0.f;
	inline static const float SHOOT_TIMER_MAX = 1.5f;
};
