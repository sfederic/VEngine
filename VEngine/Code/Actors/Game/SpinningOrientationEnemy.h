#pragma once

#include "OrientationEnemy.h"

//Enemy that rotates randomly and needs a correct oriented blade attack from player to destroy.
class SpinningOrientationEnemy : public OrientationEnemy
{
public:
	ACTOR_SYSTEM(SpinningOrientationEnemy);

	void Create() override { __super::Create(); }
	void Start() override;
	void Tick(float deltaTime) override;
	bool CanBeHit(AttackTypes attackType) override { return OrientationEnemy::CanBeHit(attackType); }

private:
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	float spinTimer = 0.f;
};
