#pragma once

#include "../PlayerUnit.h"
#include "Actors/ActorSystem.h"

//Attacks all surrounding nodes in a cross pattern.
class SurroundAttackUnit : public PlayerUnit
{
public:
	ACTOR_SYSTEM(SurroundAttackUnit);

	SurroundAttackUnit();
	virtual void AttackPattern() override;
};
