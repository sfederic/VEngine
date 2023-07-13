#include "vpch.h"
#include "LiftCrank.h"
#include "Lift.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"

void LiftCrank::Create()
{
	mesh->SetMeshFilename("gear.vmesh");
}

void LiftCrank::Start()
{
	linkedLift = dynamic_cast<Lift*>(World::GetActorByNameAllowNull(linkedLiftName));
	assert(linkedLift);
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
