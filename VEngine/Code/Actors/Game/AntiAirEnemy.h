#pragma once

#include "Enemy.h"

class MeshComponent;
class Polyboard;

class AntiAirEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(AntiAirEnemy);

	AntiAirEnemy();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	void Shoot();

	MeshComponent* mesh = nullptr;
	Polyboard* aimBeam = nullptr;

	float shootTimer = 0.f;
};
