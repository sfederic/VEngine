#include "vpch.h"
#include "MeshSliceActor.h"
#include "Components/SliceableMeshComponent.h"
#include "Gameplay/IMeshSliceReaction.h"

MeshSliceActor::MeshSliceActor()
{
	sliceableMesh = CreateComponent("Mesh", SliceableMeshComponent("cube.vmesh", "test.png"));
	//Make sure the mesh is always the root here, else the sliced meshcomponents will have wrong transforms
	SetRootComponent(sliceableMesh);
}

Properties MeshSliceActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Linked Actor", &linkedActor);
	return props;
}

void MeshSliceActor::SliceMesh(DirectX::XMVECTOR planeCenter, DirectX::XMVECTOR planeNormal)
{
	sliceableMesh->SliceMesh(planeCenter, planeNormal);

	auto actor = World::GetActorByNameAllowNull(linkedActor);
	if (actor)
	{
		auto meshSliceReaction = dynamic_cast<IMeshSliceReaction*>(actor);
		if (meshSliceReaction)
		{
			meshSliceReaction->OnSlicedLinkedActor();
		}
	}
}
