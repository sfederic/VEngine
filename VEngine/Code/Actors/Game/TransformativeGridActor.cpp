#include "vpch.h"
#include "TransformativeGridActor.h"
#include "Components/MeshComponent.h"
#include "Player.h"

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

	//Player's link effect mesh needs to be reset to reflect the changes in this actor.
	Player::system.GetOnlyActor()->SetLinkEffectMeshForTransformativeGridActors(*mesh);
}

void TransformativeGridActor::ChangeMeshShape()
{
	changeMeshActive = !changeMeshActive;

	changeMeshActive ?
		mesh->SetMeshFilename(changeMeshFilename) :
		mesh->SetMeshFilename(originalMeshFilename);

	mesh->ReCreate();
}
