#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SliceableMeshComponent;

class MeshSliceActor : public Actor
{
public:
	ACTOR_SYSTEM(MeshSliceActor);

	MeshSliceActor();
	Properties GetProps() override;

	void SliceMesh(DirectX::XMVECTOR planeCenter, DirectX::XMVECTOR planeNormal);

private:
	SliceableMeshComponent* sliceableMesh = nullptr;

	//Actor name to call IMeshSliceReaction functions from on this actor being sliced.
	std::string linkedActor;
};
