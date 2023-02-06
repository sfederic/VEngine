#include "vpch.h"
#include "MeshSplitActor.h"
#include "Components/MeshComponent.h"
#include "Core/Input.h"
#include "Core/MeshSlicer.h"

MeshSplitActor::MeshSplitActor()
{
	mesh = CreateComponent(MeshComponent("broken_building.vmesh", "test.png"), "Mesh");
	//Make sure the mesh is always the root here, else the sliced meshcomponents will have wrong transforms
	rootComponent = mesh;
}

void MeshSplitActor::Create()
{
	//mesh->SetMeshFilename("broken_building.vmesh");
}

void MeshSplitActor::SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal)
{
	if (Input::GetKeyUp(Keys::Enter))
	{
		MeshSlicer::SliceMeshViaPlane(planeCenter, planeNormal, *mesh, mesh0Verts, mesh1Verts);

		Transform originalMeshTransform = mesh->transform;
		mesh->Remove();

		{
			//@Todo: this will fail if the plane doesn't cut this mesh. d3d11 will explore on CreateBuffer
			auto mesh0 = MeshComponent::system.Add("SplitMesh0", this, MeshComponent(), false);
			mesh0->SetRastState(RastStates::noBackCull);
			mesh0->meshDataProxy.vertices = &mesh0Verts;
			mesh0->isStatic = false;
			mesh0->transform = originalMeshTransform;
			mesh0->SplitMeshCreate();
		}

		{		
			auto mesh1 = MeshComponent::system.Add("SplitMesh1", this, MeshComponent(), false);
			mesh1->SetRastState(RastStates::noBackCull);
			mesh1->meshDataProxy.vertices = &mesh1Verts;
			mesh1->isStatic = false;
			mesh1->transform = originalMeshTransform;
			mesh1->SplitMeshCreate();
		}
	}
}
