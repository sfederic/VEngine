#pragma once

#include "GridActor.h"

//Holds oil that can be spilled to form a path between a fire source and burnable object.
class OilBarrel : public GridActor
{
public:
	ACTOR_SYSTEM(OilBarrel);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void SpillOil();

	bool spillOilPerNode = true;
};
