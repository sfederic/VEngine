#include "vpch.h"
#include "LiftCrank.h"
#include "Lift.h"
#include "Core/Log.h"

void LiftCrank::Create()
{
	__super::Create();

	SetMeshFilename("gear.vmesh");
}

void LiftCrank::Start()
{
	__super::Start();

	linkedLift = dynamic_cast<Lift*>(World::GetActorByNameAllowNull(linkedLiftName));
	if (linkedLift == nullptr)
	{
		Log("Lift [%s] not found for LiftCrank [%s]", linkedLiftName.c_str(), GetName().c_str());
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
