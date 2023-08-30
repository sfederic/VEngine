#pragma once

#include "Pickup.h"

class CrystalPickup : public Pickup
{
public:
	ACTOR_SYSTEM(CrystalPickup);

	void Create() override;
	void Interact() override;
};
