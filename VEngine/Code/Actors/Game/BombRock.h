#pragma once

#include "Explosive.h"

//Explodes once unlinked. Think Yellow Pikmin.
class BombRock : public Explosive
{
public:
	ACTOR_SYSTEM(BombRock);
	DEFAULT_PROPS;

	void OnLinkDeactivate() override;
};
