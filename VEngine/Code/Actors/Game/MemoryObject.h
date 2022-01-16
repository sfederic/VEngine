#pragma once
#include "GridActor.h"

//A GridActor that needs to have weapon/item stats so that InteractTriggers
//can spawn the right data for Memories.
struct MemoryObject : GridActor
{
	ACTOR_SYSTEM(MemoryObject);

	int energyCost = 1; //How much energy objects costs to use
	int attackPoints = 0; //How much damage object does
	int defendPoints = 0; //How much damage object negates
	int weight = 0; //Movement cost while moving with item out

	MemoryObject();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	Properties GetProps() override;
};
