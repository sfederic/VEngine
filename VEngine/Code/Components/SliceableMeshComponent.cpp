#include "vpch.h"
#include "SliceableMeshComponent.h"
#include "Core/MeshSlicer.h"

void SliceableMeshComponent::SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal)
{
	std::vector<Vertex> mesh0Verts;
	std::vector<Vertex> mesh1Verts;

	MeshSlicer::SliceMeshViaPlane(planeCenter, planeNormal, *this, mesh0Verts, mesh1Verts);

	const Transform originalMeshTransform = transform;

	{
		//@Todo: this will fail if the plane doesn't cut this mesh. d3d11 will explore on CreateBuffer
		auto mesh0 = MeshComponent::system.Add("SlicedMesh0", nullptr, MeshComponent(), false);
		mesh0->SetRastState(RastStates::noBackCull);

		//Call 'new' here, can't just give the vertices a pointer to nothing
		mesh0->meshDataProxy.vertices = new std::vector<Vertex>();
		*mesh0->meshDataProxy.vertices = mesh0Verts;

		mesh0->isStatic = false;
		mesh0->transform = originalMeshTransform;
		mesh0->SplitMeshCreate();
	}

	{
		auto mesh1 = MeshComponent::system.Add("SlicedMesh1", nullptr, MeshComponent(), false);
		mesh1->SetRastState(RastStates::noBackCull);

		mesh1->meshDataProxy.vertices = new std::vector<Vertex>();
		*mesh1->meshDataProxy.vertices = mesh1Verts;

		mesh1->isStatic = false;
		mesh1->transform = originalMeshTransform;
		mesh1->SplitMeshCreate();
	}

	Remove();
}
