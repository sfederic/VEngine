#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct MeshComponent;
struct CameraComponent;

struct NormalActor : Actor
{
	ACTOR_SYSTEM(NormalActor)

	MeshComponent* mesh1;
	CameraComponent* camera;

	NormalActor();
	virtual void Tick(double deltaTime) override;
	virtual Properties GetProps();
};
