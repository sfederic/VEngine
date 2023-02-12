#include "vpch.h"
#include "MeshSliceActor.h"
#include "Components/SliceableMeshComponent.h"
#include "Core/Input.h"
#include "Core/MeshSlicer.h"

MeshSliceActor::MeshSliceActor()
{
	sliceableMesh = CreateComponent("Mesh", SliceableMeshComponent("cube.vmesh", "test.png"));
	//Make sure the mesh is always the root here, else the sliced meshcomponents will have wrong transforms
	rootComponent = sliceableMesh;
}

void MeshSliceActor::SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal)
{
	sliceableMesh->SliceMesh(planeCenter, planeNormal);
}
