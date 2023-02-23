#pragma once

#include "Gear.h"

//@Todo: because there's now a Canon and Rifle, make the canon more of an explosive weapon.

class CanonGear : public Gear
{
public:
	CanonGear() : Gear("CanonGear") {}
	void Use() override;
};
