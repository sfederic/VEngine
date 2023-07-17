#pragma once

#include "EntranceTrigger.h"

class EntranceInteractTrigger : public EntranceTrigger
{
public:
	ACTOR_SYSTEM(EntranceInteractTrigger);

	void Tick(float deltaTime) override;
};
