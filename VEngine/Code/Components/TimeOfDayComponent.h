#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct TimeOfDayComponent : Component
{	
	COMPONENT_SYSTEM(TimeOfDayComponent);

	int activeStartHour = 0;
	int activeStartMinute = 0;
	int activeEndHour = 0;
	int activeEndMinute = 0;

	TimeOfDayComponent();
	virtual Properties GetProps() override;
	bool CheckIfActiveAtCurrentTime();
};
