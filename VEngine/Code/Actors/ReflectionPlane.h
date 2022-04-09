#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct ReflectionPlane : Actor
{
	ACTOR_SYSTEM(ReflectionPlane);

	virtual Properties GetProps() override;
};
