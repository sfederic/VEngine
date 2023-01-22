#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SkeletalMeshComponent;

class AnimCube : public Actor
{
public:
	ACTOR_SYSTEM(AnimCube);

	AnimCube();
	void Create() override;
	void Start() override;
	Properties GetProps() override;

private:
	SkeletalMeshComponent* skeletalMesh = nullptr;
};
