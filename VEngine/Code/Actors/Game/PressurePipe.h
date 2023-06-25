#pragma once

#include "GridActor.h"

class MeshComponent;

//Produces a water/air/steam flow to hold another grid actor at the apex of that flow.
class PressurePipe : public GridActor
{
public:
	ACTOR_SYSTEM(PressurePipe);

	PressurePipe();
	void Create() override;

private:
	MeshComponent* flowMesh = nullptr;
};
