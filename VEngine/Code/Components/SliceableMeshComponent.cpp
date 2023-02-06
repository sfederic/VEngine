#include "vpch.h"
#include "SliceableMeshComponent.h"
#include "Core/MeshSlicer.h"

void SliceableMeshComponent::SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal)
{
	std::vector<Vertex> mesh0Verts;
	std::vector<Vertex> mesh1Verts;

	MeshSlicer::SliceMeshViaPlane(planeCenter, planeNormal, *this, mesh0Verts, mesh1Verts);

	const Transform originalMeshTransform = transform;

	//@Todo: this will fail if the plane doesn't cut this mesh. d3d11 will explore on CreateBuffer
	MeshSlicer::CreateSlicedMesh(mesh0Verts, originalMeshTransform);
	MeshSlicer::CreateSlicedMesh(mesh1Verts, originalMeshTransform);

	Remove();
}
