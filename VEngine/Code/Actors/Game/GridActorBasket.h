#pragma once

#include "GridActor.h"

//Generic 'basket/box' that holds a type of GridActor by name reference and spawns it when the player links
//to this. (Think of it like a fruit basket, taking singular fruit out one at a time.)
class GridActorBasket : public GridActor
{
public:
	ACTOR_SYSTEM(GridActorBasket);

	Properties GetProps() override;
	void OnLinkActivate() override;

private:
	std::string gridActorSystemNameToSpawnFrom;
};
