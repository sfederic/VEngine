#pragma once

#include "Particle/Polyboard.h"

class SwordBeam : public Polyboard
{
public:
	COMPONENT_SYSTEM(SwordBeam);

	SwordBeam();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	void MoveAlongDirection(float deltaTime);

public:
	XMVECTOR movementDirection = XMVectorZero();

private:
	float lifetime = 2.0f;
};
