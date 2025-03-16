
#include "PhysicsDoorGridActor.h"
import Components.MeshComponent;
import Actors.Game.Player;
#include "Actors/Game/Grid.h"
#include "Physics/HitResult.h"

void PhysicsDoorGridActor::Create()
{
	__super::Create();
	SetMeshFilename("plane.vmesh");
	canFall = false;
}

Properties PhysicsDoorGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void PhysicsDoorGridActor::OnLinkMove()
{
	__super::OnLinkMoveBack();

	if (!hasBeenUnhinged)
	{
		hasBeenUnhinged = true;

		for (auto mesh : GetComponents<MeshComponent>())
		{
			mesh->canBeLinkedTo = false;
			mesh->ignoreGridRaycasts = true;
			mesh->ReCreateAsPhysicsActor(false);
			mesh->AddForce(GetForwardVectorV());
		}

		Player::system.GetOnlyActor()->ResetLinkedGridActor();

		HitResult hit(this);
		Grid::system.GetOnlyActor()->RecalcAllNodes(hit, true);
	}
}
