#pragma once

#include "../PlayerUnit.h"
#include "Actors/ActorSystem.h"

class AttackUnit : public PlayerUnit
{
public:
	ACTOR_SYSTEM(AttackUnit);

	AttackUnit();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
