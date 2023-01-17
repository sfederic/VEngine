#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class MeshComponent;

class AnimCube : public Actor
{
public:
	ACTOR_SYSTEM(AnimCube);

	AnimCube();
	void Create() override;
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};
