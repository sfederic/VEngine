#pragma once

#include "GridActor.h"

//Door that incrementally opens when a GearSet turns.
class GearSetDoor : public GridActor
{
public:
	ACTOR_SYSTEM(GearSetDoor);

	void Create() override;
	void Start() override { __super::Start(); }
	void Tick(float deltaTime) override { __super::Tick(deltaTime); }

	//Number of increments before the door is fully opened.
	static const int maxNumIncrements = 10;
};
