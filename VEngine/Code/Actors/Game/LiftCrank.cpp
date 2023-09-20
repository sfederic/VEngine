#include "vpch.h"
#include "LiftCrank.h"
#include "Lift.h"

void LiftCrank::Create()
{
	__super::Create();

	SetMeshFilename("gear.vmesh");
}

void LiftCrank::Start()
{
	__super::Start();

	linkedLift = World::GetActorByNameAndLogCast<Lift>(linkedLiftName);
	if (linkedLift != nullptr)
	{
		SetPlayerFocusGridActor(linkedLift);
	}
}

Properties LiftCrank::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Lift", &linkedLiftName);
	props.Add("Move Direction", &moveDirection);
	return props;
}

void LiftCrank::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	const XMVECTOR moveDirectionV = XMLoadFloat3(&moveDirection);
	MoveLinkedLiftAndContainedActors(moveDirectionV);
}

void LiftCrank::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	const XMVECTOR moveDirectionV = XMLoadFloat3(&moveDirection);
	MoveLinkedLiftAndContainedActors(-moveDirectionV);
}

void LiftCrank::MoveLinkedLiftAndContainedActors(const XMVECTOR direction)
{
	if (linkedLift)
	{
		linkedLift->AddNextPosition(direction);

		for (auto containedGridActor : linkedLift->GetGridActorsContainedInLiftTrigger())
		{
			containedGridActor->AddNextPosition(direction);
		}
	}
}
