#pragma once

#include "GridActor.h"

//Places a crystal from global props "inventory" onto this actor.
class CrystalPlacement : public GridActor
{
public:
	ACTOR_SYSTEM(CrystalPlacement);

	Properties GetProps() override;
	void Interact() override;

private:
	MeshComponent* crystalMesh = nullptr;

	bool hasCrystalBeenPlaced = false;
};
