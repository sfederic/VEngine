#pragma once

#include "Gear.h"

class ShieldGear : public Gear
{
public:
	ShieldGear() : Gear("ShieldGear") {}
	void Use() override;
};
