#include "vpch.h"
#include "LiftCrank.h"
#include "Lift.h"

void LiftCrank::Create()
{
	SetMeshFilename("gear.vmesh");
}

void LiftCrank::Start()
{
	__super::Start();

	linkedLift = dynamic_cast<Lift*>(World::GetActorByNameAllowNull(linkedLiftName));
	assert(linkedLift);
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
	const XMVECTOR moveDirectionV = XMLoadFloat3(&moveDirection);
	MoveLinkedLiftAndContainedActors(moveDirectionV);
}

void LiftCrank::OnLinkRotateRight()
{
	const XMVECTOR moveDirectionV = XMLoadFloat3(&moveDirection);
	MoveLinkedLiftAndContainedActors(-moveDirectionV);
}

void LiftCrank::MoveLinkedLiftAndContainedActors(const XMVECTOR direction)
{
	linkedLift->AddNextPosition(direction);

	for (auto containedGridActor : linkedLift->GetGridActorsContainedInLiftTrigger())
	{
		containedGridActor->AddNextPosition(direction);
	}
}
