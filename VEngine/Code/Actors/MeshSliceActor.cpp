#include "vpch.h"
#include "MeshSliceActor.h"
#include "Components/SliceableMeshComponent.h"
#include "Core/Input.h"
#include "Core/MeshSlicer.h"
#include "Gameplay/IMeshSliceReaction.h"

MeshSliceActor::MeshSliceActor()
{
	sliceableMesh = CreateComponent("Mesh", SliceableMeshComponent("cube.vmesh", "test.png"));
	//Make sure the mesh is always the root here, else the sliced meshcomponents will have wrong transforms
	rootComponent = sliceableMesh;
}

Properties MeshSliceActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Linked Actor", &linkedActor);
	return props;
}

void MeshSliceActor::SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal)
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
