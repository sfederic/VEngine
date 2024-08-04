#include "vpch.h"
#include "PhysicsDoorGridActor.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"

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
			mesh->SetPhysicsStatic(false);
			mesh->ReCreateAsPhysicsActor();
		}

		Player::system.GetOnlyActor()->ResetLinkedGridActor();
	}
}
