#pragma once

#include "Enemy.h"

class MeshComponent;

//Boss that shoots out walls at player to dodge through (think like that japanase 
//game show. 'Brain Wall' was it?) and also is a wall.
class WallBoss : public Enemy
{
public:
	ACTOR_SYSTEM(WallBoss);

	WallBoss();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	void SpawnWallProjectile();
	void MoveWallProjectile(float deltaTime);
	void RemoveAllWallProjectileParts();

	MeshComponent* baseMesh = nullptr;
	std::vector<MeshComponent*> wallProjectileParts;
	float wallSpawnTimer = wallTimersMax;
	float wallProjectileDestroyTimer = 0.f;
	static inline const float wallTimersMax = 4.f;
};
