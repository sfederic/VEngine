#pragma once

#include "GridActor.h"
#include "Core/Properties.h"

class BoxTriggerComponent;

//Spinning saw blade that is powered by a crystal
class SawBlade : public GridActor
{
public:
	ACTOR_SYSTEM(SawBlade);

	SawBlade();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void PowerUp() override;
	void PowerDown() override;

private:
	void SliceSurroundingGridActors();

	BoxTriggerComponent* sliceTrigger = nullptr;

	bool isCutting = false;
};
