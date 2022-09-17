#pragma once

#include "InteractActor.h"

class DoorSwitch : public InteractActor
{
public:
	ACTOR_SYSTEM(DoorSwitch);

	DoorSwitch() {}
	virtual Properties GetProps() override;
	virtual void Interact() override;

private:
	std::string linkedDoorName;
};
