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
	void Start() override;
	Properties GetProps() override;

	void Activate() override;
	void Deactivate() override;

private:
	MeshComponent* flowMesh = nullptr;

	bool isActive = false;
};
