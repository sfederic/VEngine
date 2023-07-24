#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//Spinning saw blade that is powered by a crystal
class SawBlade : public GridActor
{
public:
	ACTOR_SYSTEM(SawBlade);

	SawBlade();
	void Create() override;
	void Tick(float deltaTime) override;

	void PowerUp() override;
	void PowerDown() override;

private:
	void SliceSurroundingGridActors();

	BoxTriggerComponent* sliceTrigger = nullptr;

	bool isCutting = false;
};
