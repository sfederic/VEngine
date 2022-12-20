#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;

class TurretBullet : public Actor
{
public:
	ACTOR_SYSTEM(TurretBullet);

	TurretBullet();
	void Tick(float deltaTime) override;
	Properties GetProps() override { return __super::GetProps(); }

	void SetMovementDirection(XMVECTOR movementDirection_) { movementDirection = movementDirection_; }

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	XMVECTOR movementDirection = XMVectorZero();
	float moveSpeed = 5.0f;
};
