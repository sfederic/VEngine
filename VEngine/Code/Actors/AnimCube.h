#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SkeletalMeshComponent;
class SocketMeshComponent;

class AnimCube : public Actor
{
public:
	ACTOR_SYSTEM(AnimCube);

	AnimCube();
	void PostCreate() override;
	void Start() override;
	Properties GetProps() override;

private:
	SkeletalMeshComponent* skeletalMesh = nullptr;
	SocketMeshComponent* socketMesh = nullptr;
};
