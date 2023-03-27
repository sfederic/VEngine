#pragma once

#include "../PlayerUnit.h"
#include "Actors/ActorSystem.h"

//Attacks directly in front of itself in its facing direction.
class AttackUnit : public PlayerUnit
{
public:
	ACTOR_SYSTEM(AttackUnit);

	AttackUnit();
	virtual void AttackPattern() override;
};
