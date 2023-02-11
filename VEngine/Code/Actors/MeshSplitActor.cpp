#include "vpch.h"
#include "MeshSplitActor.h"
#include "Components/SliceableMeshComponent.h"
#include "Core/Input.h"
#include "Core/MeshSlicer.h"

MeshSplitActor::MeshSplitActor()
{
	sliceableMesh = CreateComponent("Mesh", SliceableMeshComponent("cube.vmesh", "test.png"));
	//Make sure the mesh is always the root here, else the sliced meshcomponents will have wrong transforms
	rootComponent = sliceableMesh;
}

void MeshSplitActor::SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal)
{
	sliceableMesh->SliceMesh(planeCenter, planeNormal);
}
