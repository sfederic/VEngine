#pragma once

#include "GridActor.h"

//This Drill is about putting it on top of actors and spinning it like a beyblade to break them.
class Drill : public GridActor
{
public:
	ACTOR_SYSTEM(Drill);

	void Create() override;
	void OnLinkRotate() override;

private:
	void DrillThroughBelowGridActor();

	//For each link rotation, this will increment. When it's above a threshold, break the below grid actor.
	int rotateDrillIncrement = 0;
};
