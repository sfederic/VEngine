#pragma once

#include "Explosive.h"

//Thanks Ocarina.
class BombFlower : public Explosive
{
public:
	ACTOR_SYSTEM(BombFlower);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void OnLinkActivate() override;

private:
	float explodeTimer = 0.f;
	bool setToExplode = false;
};
