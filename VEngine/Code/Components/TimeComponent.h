#pragma once
#include "Component.h"
#include "ComponentSystem.h"

//Holds time of day data for actors.
struct TimeComponent : Component
{	
	COMPONENT_SYSTEM(TimeComponent);

	int activeStartHour = 0;
	int activeStartMinute = 0;
	int activeEndHour = 0;
	int activeEndMinute = 0;

	TimeComponent();
	virtual Properties GetProps() override;
	bool CheckIfActiveAtCurrentTime();
};
