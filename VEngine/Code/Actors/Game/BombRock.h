#pragma once

#include "Explosive.h"

//Explodes once unlinked. Think Yellow Pikmin.
class BombRock : public Explosive
{
public:
	ACTOR_SYSTEM(BombRock);
	Properties GetProps() override;

	void OnLinkDeactivate() override;
};
