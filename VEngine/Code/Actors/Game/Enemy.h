#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class Enemy : public Actor
{
public: 
	ACTOR_SYSTEM(Enemy);

	Enemy();
	virtual Properties GetProps() override;
};
