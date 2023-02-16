#pragma once

#include "Gear.h"

class SwordGear : public Gear
{
public:
	SwordGear() : Gear("SwordGear") {}
	virtual void Use() override;
};
