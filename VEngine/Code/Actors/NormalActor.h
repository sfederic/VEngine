#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct MeshComponent;
struct CameraComponent;
struct BoxTriggerComponent;

struct NormalActor : Actor
{
	ACTOR_SYSTEM(NormalActor)

	MeshComponent* mesh1;
	MeshComponent* mesh2;
	CameraComponent* camera;
	BoxTriggerComponent* boxTrigger;
	struct Material* nextMaterial;

	NormalActor();
	virtual void Tick(double deltaTime) override;
	virtual Properties GetProps();
};
