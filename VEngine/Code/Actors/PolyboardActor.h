#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class Polyboard;

class PolyboardActor : public Actor
{
public:
	ACTOR_SYSTEM(PolyboardActor);

	PolyboardActor();
	Properties GetProps() override;

private:
	Polyboard* polyboard = nullptr;
};
