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

	XMVECTOR falldownDirectionWhenDestroyed = XMVectorZero();

	//For visual effects checks (e.g. if an airborne enemy is falling after being shot down)
	bool isInDestroyedState = false;
};
