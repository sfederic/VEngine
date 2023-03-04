#pragma once

#include "Enemy.h"

class MeshComponent;
class BoxTriggerComponent;
class ParticleEmitter;

//@Todo: I'd love a visual effect like a growth/vines coming out of the enemy into the structure/floor
//its landed on once it has landed.

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
