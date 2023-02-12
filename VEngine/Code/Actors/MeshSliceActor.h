#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SliceableMeshComponent;

class MeshSliceActor : public Actor
{
public:
	ACTOR_SYSTEM(MeshSliceActor);

	MeshSliceActor();
	Properties GetProps() { return __super::GetProps(); }

	void SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal);

private:
	SliceableMeshComponent* sliceableMesh = nullptr;
};
