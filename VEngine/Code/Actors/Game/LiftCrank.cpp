#include "vpch.h"
#include "LiftCrank.h"
#include "Lift.h"
#include "Core/VMath.h"

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
	return props;
}

void LiftCrank::OnLinkRotateLeft()
{
	MoveLinkedLiftAndContainedActors(VMath::GlobalUpVector());
}

void LiftCrank::OnLinkRotateRight()
{
	MoveLinkedLiftAndContainedActors(-VMath::GlobalUpVector());
}

void LiftCrank::MoveLinkedLiftAndContainedActors(XMVECTOR moveDirection)
{
	linkedLift->AddNextPosition(moveDirection);

	for (auto containedGridActor : linkedLift->GetGridActorsContainedInLiftTrigger())
	{
		containedGridActor->AddNextPosition(moveDirection);
	}
}
