#pragma once

#include "../Unit.h"

class MemoryImpartUnit : public Unit
{
public:
	ACTOR_SYSTEM(MemoryImpartUnit);

	MemoryImpartUnit();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;

	virtual void ImpartMemoryToPlayerOnAttack() override;
};
