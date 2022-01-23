#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Just for rough in-engine physics testing (without PhysX)
struct PhysicsActor : Actor
{
	ACTOR_SYSTEM(PhysicsActor);

	MeshComponent* sphereMesh = nullptr;

	XMVECTOR gravityVector;
	XMVECTOR moveVector;

	PhysicsActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	bool GoOverAllTris();
};
