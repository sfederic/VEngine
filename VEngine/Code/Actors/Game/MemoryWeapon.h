#pragma once
#include "GridActor.h"
#include "Gameplay/WeaponData.h"

//A GridActor that needs to have weapon/item stats so that InteractTriggers
//can spawn the right data for Memories.
struct MemoryWeapon : GridActor
{
	ACTOR_SYSTEM(MemoryWeapon);

	WeaponData weaponData;

	MemoryWeapon();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	Properties GetProps() override;
};
