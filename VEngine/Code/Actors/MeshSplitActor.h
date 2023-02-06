#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SliceableMeshComponent;

class MeshSplitActor : public Actor
{
public:
	ACTOR_SYSTEM(MeshSplitActor);

	MeshSplitActor();
	Properties GetProps() { return __super::GetProps(); }

	void SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal);

private:
	SliceableMeshComponent* sliceableMesh = nullptr;
};
