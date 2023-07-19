#pragma once

#include "GridActor.h"

class CrystalPickup : public GridActor
{
public:
	ACTOR_SYSTEM(CrystalPickup);

	void Create() override;
	void Interact() override;
};
