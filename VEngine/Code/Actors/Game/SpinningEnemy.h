#pragma once

#include "Enemy.h"

//An everlasting gobstopper enemy that rotates randomly in increments.
class SpinningEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(SpinningEnemy);

	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void CreateMesh(const std::string name, XMFLOAT3 position);

	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	float spinTimer = 0.f;
};
