#pragma once

#include "Enemy.h"

class MeshComponent;
class BossHealthBar;
struct Polyboard;

class RamielBoss : public Enemy
{
public:
	ACTOR_SYSTEM(RamielBoss);

	RamielBoss();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void FireChargeBeamAtPlayer();

	BossHealthBar* bossHealthBar = nullptr;
	Polyboard* attackBeam = nullptr;
	MeshComponent* mesh = nullptr;
	XMFLOAT4 ambientColour = XMFLOAT4(0.1f, 0.1f, 0.9f, 1.f);
	float chargeAttackTimer = 0.f;
};
