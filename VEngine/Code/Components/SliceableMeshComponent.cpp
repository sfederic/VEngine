#include "vpch.h"
#include "SliceableMeshComponent.h"
#include "Core/MeshSlicer.h"
#include "Actors/SplitMesh.h"

void SliceableMeshComponent::SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal)
{
	std::vector<Vertex> mesh0Verts;
	std::vector<Vertex> mesh1Verts;

	MeshSlicer::SliceMeshViaPlane(planeCenter, planeNormal, *this, mesh0Verts, mesh1Verts);

	const Transform originalMeshTransform = transform;

	//@Todo: this will fail if the plane doesn't cut this mesh. d3d11 will explode on CreateBuffer
	auto splitMesh0 = SplitMesh::system.Add(originalMeshTransform);
	splitMesh0->CreateSplitMesh(mesh0Verts, this);

	auto splitMesh1 = SplitMesh::system.Add(originalMeshTransform);
	splitMesh1->CreateSplitMesh(mesh1Verts, this);

	Remove();
}
