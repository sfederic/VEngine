#pragma once
#include "GridActor.h"

//A GridActor that needs to have weapon/item stats so that InteractTriggers
//can spawn the right data for Memories.
struct MemoryObject : GridActor
{
	ACTOR_SYSTEM(MemoryObject);

	MemoryObject();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	Properties GetProps() override;
};
