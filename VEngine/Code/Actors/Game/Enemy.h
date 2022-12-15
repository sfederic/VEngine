#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class Enemy : public Actor
{
public: 
	ACTOR_SYSTEM(Enemy);

	Enemy();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
