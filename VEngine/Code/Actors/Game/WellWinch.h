#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//@Todo: this was hard to program a bucket going up and down from a winch. Maybe delete.
class WellWinch : public GridActor
{
public:
	ACTOR_SYSTEM(WellWinch);

	WellWinch();
	void Create() override;

private:
	BoxTriggerComponent* bucketTrigger = nullptr;
};
