#pragma once

#include "ActivateOnGlobalProp.h"

class ChurchLockPropActivator : public ActivateOnGlobalProp
{
public:
	ACTOR_SYSTEM(ChurchLockPropActivator);

	void Activate() override;
};
