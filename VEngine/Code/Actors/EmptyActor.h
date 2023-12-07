#pragma once

#include "Actor.h"
#include "ActorSystem.h"

//Empty actor used for targets or in-level spatial references or whatever.
class EmptyActor : public Actor
{
public:
	ACTOR_SYSTEM(EmptyActor);

	EmptyActor();
	Properties GetProps() override;
};
