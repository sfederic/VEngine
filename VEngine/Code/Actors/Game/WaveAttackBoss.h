#pragma once

#include "Enemy.h"

class MeshComponent;

//Boss that shoots a large directional plane at the player, forcing them to hide behind obstacles.
class WaveAttackBoss : public Enemy
{
public:
	ACTOR_SYSTEM(WaveAttackBoss);

	WaveAttackBoss();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	void ShootAreaAttack();
	void RaycastCheckOnAreaAttackDimensions();

	MeshComponent* baseMesh = nullptr;
	MeshComponent* areaAttackMesh = nullptr;

	float shootTimer = 0.f;
	float areaAttackMeshLifetimeTimer = 0.f;
};
