#pragma once

#include "GridActor.h"

class Burnable : public GridActor
{
public:
	ACTOR_SYSTEM(Burnable);

	Properties GetProps() override;
	void Burn() override;

private:
	//Give a delay to objects burning so that player refocus isn't immediate.
	void BurnDelay();

	float burnSpriteTimer = 0.f;
	bool onFire = false;
};
