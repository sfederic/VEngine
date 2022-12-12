#pragma once

#include "../PlayerUnit.h"
#include "Actors/ActorSystem.h"

class DoubleAttackFusionUnit : public PlayerUnit
{
public:
	ACTOR_SYSTEM(DoubleAttackFusionUnit);

	virtual std::vector<Unit*> AttackPattern() override;
};
