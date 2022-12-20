#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class TurretBullet : public Actor
{
public:
	ACTOR_SYSTEM(TurretBullet);

	TurretBullet();
	void Tick(float deltaTime) override;
	Properties GetProps() override { return __super::GetProps(); }

	void SetMovementDirection(XMVECTOR movementDirection_) { movementDirection = movementDirection_; }

private:
	XMVECTOR movementDirection = XMVectorZero();

	float moveSpeed = 5.0f;
};
