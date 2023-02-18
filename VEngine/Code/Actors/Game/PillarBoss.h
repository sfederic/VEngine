#pragma once

#include "Enemy.h"

//Pillar Enemy that spins around and shoots lasers.
class PillarBoss : public Enemy
{
public:
	ACTOR_SYSTEM(PillarBoss);

	PillarBoss();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	MeshComponent* baseMesh = nullptr;
};
