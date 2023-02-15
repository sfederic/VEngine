#pragma once

#include "PlayerGear.h"

class ShieldGear : public PlayerGear
{
public:
	ShieldGear() : PlayerGear("ShieldGear") {}
	void Use() override;
};
