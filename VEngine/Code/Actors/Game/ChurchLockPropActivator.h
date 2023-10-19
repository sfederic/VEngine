#pragma once

#include "ActivateOnGlobalProp.h"

//@Todo: there's something wrong with this class/file. It always pops up in strange build errors
//like changing to Release or precompiled header issues. I wonder what it is

class ChurchLockPropActivator : public ActivateOnGlobalProp
{
public:
	ACTOR_SYSTEM(ChurchLockPropActivator);

	void Activate() override;
};
