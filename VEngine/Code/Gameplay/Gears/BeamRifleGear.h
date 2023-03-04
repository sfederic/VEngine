#pragma once

#include "Gear.h"

//@Todo: when the beam rifle spawns its polyboard, the board isn't orienting itself correctly because
//the player camera is set in a manner behind the player.

//Beam rifle shoots through enemies like a skewer.
class BeamRifleGear : public Gear
{
public:
	BeamRifleGear() : Gear("BeamRifle") {}
	virtual void Use() override;
};
