
#include "TransformativeGridActor.h"
import Components.MeshComponent;
#include "Player.h"

void TransformativeGridActor::Create()
{
	__super::Create();

	_mesh->SetMeshFilename(originalMeshFilename);
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
	Player::system.GetOnlyActor()->SetLinkEffectMeshForTransformativeGridActors(*_mesh);
}

void TransformativeGridActor::ChangeMeshShape()
{
	changeMeshActive = !changeMeshActive;

	changeMeshActive ?
		_mesh->SetMeshFilename(changeMeshFilename) :
		_mesh->SetMeshFilename(originalMeshFilename);

	_mesh->ReCreate();
}
