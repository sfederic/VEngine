#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;
class HealthWidget;

class DestructibleObject : public Actor
{
public:
	ACTOR_SYSTEM(DestructibleObject);

	DestructibleObject() {}
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override { return __super::GetProps(); }

protected:
	MeshComponent* mesh = nullptr;

	HealthWidget* healthWidget = nullptr;

	int health = 1;

	XMVECTOR falldownDirectionWhenDestroyed = XMVectorZero();

	//For visual effects checks (e.g. if an airborne enemy is falling after being shot down)
	bool isInDestroyedState = false;
};
