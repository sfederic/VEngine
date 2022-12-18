#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

class Enemy : public Actor
{
public: 
	ACTOR_SYSTEM(Enemy);

	Enemy();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};
