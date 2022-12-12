#pragma once

#include "../PlayerUnit.h"
#include "Actors/ActorSystem.h"

//Attacks all surrounding nodes in a square pattern.
class SurroundAttackUnit : public PlayerUnit
{
public:
	ACTOR_SYSTEM(SurroundAttackUnit);

	SurroundAttackUnit();
	virtual std::vector<Unit*> AttackPattern() override;
};
