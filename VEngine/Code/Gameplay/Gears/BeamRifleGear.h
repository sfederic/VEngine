#pragma once

#include "Gear.h"

//Beam rifle shoots through enemies like a skewer.
class BeamRifleGear : public Gear
{
public:
	BeamRifleGear() : Gear("BeamRifle") {}
	virtual void Use() override;
};
