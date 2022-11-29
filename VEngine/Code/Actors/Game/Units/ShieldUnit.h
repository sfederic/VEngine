#pragma once

#include "../Unit.h"

//Unit that holds a shield in front of itself.
class ShieldUnit : public Unit
{
public:
	ACTOR_SYSTEM(ShieldUnit);

	ShieldUnit();
	//virtual void Start() override { __super::Start(); }
	//virtual void Tick(float deltaTime) override { __super::Tick(deltaTime); }
	//Properties GetProps() { return __super::GetProps(); }
};
