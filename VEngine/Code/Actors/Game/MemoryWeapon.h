#pragma once
#include "GridActor.h"
#include "Gameplay/WeaponData.h"

struct BoxTriggerComponent;
struct InteractWidget;

//A GridActor that needs to have weapon/item stats so that InteractTriggers
//can spawn the right data for Memories.
struct MemoryWeapon : GridActor
{
	ACTOR_SYSTEM(MemoryWeapon);

	BoxTriggerComponent* memoryTrigger = nullptr;

	InteractWidget* interactWidget = nullptr;

	WeaponData weaponData;

private:
	bool isBeingInteractedWith = false;

public:
	MemoryWeapon();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	Properties GetProps() override;
};
