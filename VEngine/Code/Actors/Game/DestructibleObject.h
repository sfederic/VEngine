#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;

class DestructibleObject : public Actor
{
public:
	ACTOR_SYSTEM(DestructibleObject);

	DestructibleObject();
	Properties GetProps() override { return __super::GetProps(); }

protected:
	MeshComponent* mesh = nullptr;

	int health = 1;
};
