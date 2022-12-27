#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SkyBox : public Actor
{
public:
	ACTOR_SYSTEM(SkyBox);

	SkyBox();
	Properties GetProps() override;
};
