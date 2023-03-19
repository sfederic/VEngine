#pragma once

#include "Enemy.h"

class MeshComponent;
class BoxTriggerComponent;
class ParticleEmitter;

//Enemy that crash lands into shit from outside the map, basically a gimmick spawning enemy.
//Think like the Sayians from DBZ.
class CrashLandEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(CrashLandEnemy);

	CrashLandEnemy();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;

	void SetCrashLandDirection(XMVECTOR dir) { crashLandDirection = dir; }

private:
	MeshComponent* mesh = nullptr;
	BoxTriggerComponent* boxTrigger = nullptr; 
	ParticleEmitter* smokeTrailEmitter = nullptr;

	XMVECTOR crashLandDirection = XMVectorZero();
};
