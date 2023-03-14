#pragma once

#include "Gear.h"

class MachineGunGear : public Gear
{
public:
	MachineGunGear() : Gear("MachineGun") 
	{
		gearInputType = GearInputType::Hold;
	}

	virtual void Use() override;
};
