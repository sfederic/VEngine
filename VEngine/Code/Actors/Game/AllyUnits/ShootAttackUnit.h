#pragma once

#include "../PlayerUnit.h"
#include "Actors/ActorSystem.h"

//Attack in a straight line and damage the first enemy on contact.
class ShootAttackUnit : public PlayerUnit
{
public:
	ACTOR_SYSTEM(ShootAttackUnit);

	ShootAttackUnit();
	virtual void AttackPattern() override;
};
