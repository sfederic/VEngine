#pragma once

#include "../Unit.h"

class LineAttackUnit : public Unit
{
public:
	ACTOR_SYSTEM(LineAttackUnit);

	LineAttackUnit();
	virtual Properties GetProps() override { return __super::GetProps(); }
};
