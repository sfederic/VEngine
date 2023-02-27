#pragma once

#include "Enemy.h"

class Polyboard;
class MeshComponent;

struct LaserData
{
	Polyboard* beam = nullptr;
	MeshComponent* mesh = nullptr;

	void SetBeamPointsFromMesh();
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
	void CalcLaserData(uint32_t numLasersToSpawn);

	MeshComponent* baseMesh = nullptr;
	std::vector<LaserData> laserDatas;
};
