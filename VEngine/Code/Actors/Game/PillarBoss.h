#pragma once

#include "Enemy.h"

struct Polyboard;
class MeshComponent;

struct LaserData
{
	XMVECTOR position = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR shootDirection = XMVectorZero();
	Polyboard* beam = nullptr;
	MeshComponent* mesh = nullptr;
};

//Pillar Enemy that spins around and shoots lasers.
class PillarBoss : public Enemy
{
public:
	ACTOR_SYSTEM(PillarBoss);

	PillarBoss();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;

private:
	void CalcLaserPositions();

	MeshComponent* baseMesh = nullptr;
	std::vector<LaserData> laserPositions;
};
