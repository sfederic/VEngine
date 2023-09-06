#pragma once

#include "OrientationLock.h"

class ClockHand : public OrientationLock
{
public:
	ACTOR_SYSTEM(ClockHand);

	ClockHand();
	Properties GetProps() override;
};
