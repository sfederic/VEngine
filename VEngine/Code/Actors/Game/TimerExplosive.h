#pragma once

#include "Explosive.h"

//Think bomb flower from ocarina.
class TimerExplosive : public Explosive
{
public:
	ACTOR_SYSTEM(TimerExplosive);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void OnLinkActivate() override;

private:
	float explodeTimer = 0.f;
	bool setToExplode = false;
};
