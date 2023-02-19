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
	void Start() override;
	void Tick(float deltaTime) override;

private:
	void SpawnWallProjectile();
	void MoveWallProjectile(float deltaTime);

	MeshComponent* baseMesh = nullptr;
	float wallSpawnTimer = 0.f;

	std::vector<MeshComponent*> wallProjectileParts;
};
