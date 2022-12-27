#pragma once

#include "../Actor.h"

//Base class for actors that player can interact with on map.
class InteractActor : public Actor
{
public:
	virtual void Interact() = 0;
};
