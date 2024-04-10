#include "vpch.h"
#include "TransformativeGridActor.h"
#include "Components/MeshComponent.h"

void TransformativeGridActor::Create()
{
	__super::Create();

	mesh->SetMeshFilename(originalMeshFilename);
}

Properties TransformativeGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Change Mesh", &changeMeshFilename).autoCompletePath = "/Meshes/";
	return props;
}

void TransformativeGridActor::OnLinkMove()
{
	__super::OnLinkMove();
	ChangeMeshShape();
}

void TransformativeGridActor::ChangeMeshShape()
{
	changeMeshActive = !changeMeshActive;

	changeMeshActive ?
		mesh->SetMeshFilename(changeMeshFilename) :
		mesh->SetMeshFilename(originalMeshFilename);

	mesh->ReCreate();
}
