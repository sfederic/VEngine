#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

class TurretBullet : public Actor
{
public:
	ACTOR_SYSTEM(TurretBullet);

	TurretBullet();
	void Tick(float deltaTime) override;
	Properties GetProps() override { return __super::GetProps(); }

	void SetMovementDirection(XMVECTOR movementDirection_) { movementDirection = movementDirection_; }

private:
	void DestroyIfAliveForTooLong(float deltaTime);

	BoxTriggerComponent* boxTrigger = nullptr;

	XMVECTOR movementDirection = XMVectorZero();
	float moveSpeed = 5.0f;

	float destroyTimer = 0.f;
};
