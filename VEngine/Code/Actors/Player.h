#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct MeshComponent;
struct CameraComponent;

struct Player : Actor
{
	ACTOR_SYSTEM(Player)

	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

	Player();
	virtual void Start() override;
	virtual void Tick(double deltaTime) override;
	virtual Properties GetProps() override;
};
