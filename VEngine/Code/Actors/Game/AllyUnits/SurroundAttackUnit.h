#pragma once

#include "../PlayerUnit.h"
#include "Actors/ActorSystem.h"

class SurroundAttackUnit : public PlayerUnit
{
public:
	ACTOR_SYSTEM(SurroundAttackUnit);

	SurroundAttackUnit();
	virtual void AttackPattern() override;
};
