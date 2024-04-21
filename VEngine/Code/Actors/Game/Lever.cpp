#include "vpch.h"
#include "Lever.h"

void Lever::Create()
{
	__super::Create();

	SetMeshFilename("lever.vmesh");

	canBeMovedInLink = false;
	canBeRotatedRollZAxis = false;
	canBeRotatedYawYAxis = false;
}

void Lever::Start()
{
	__super::Start();

	actorToActivate = World::GetActorByNameAllowNull(actorToActivateName);
}

Properties Lever::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Actor To Activate", &actorToActivateName).useActorsAutoComplete = true;
	props.Add("Lever Active", &isLeverActive);
	return props;
}

void Lever::OnLinkRotate()
{
	__super::OnLinkRotate();

	isLeverActive = !isLeverActive;

	if (actorToActivate)
	{
		if (isLeverActive)
		{
			actorToActivate->Activate();
		}
		else
		{
			actorToActivate->Deactivate();
		}
	}
}
