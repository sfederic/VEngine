#pragma once

#include "Gear.h"

class CanonGear : public Gear
{
public:
	CanonGear() : Gear("CanonGear") {}
	void Use() override;
};
