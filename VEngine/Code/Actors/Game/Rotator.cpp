
#include "Rotator.h"
#include "Core/Log.h"
#include "Grid.h"

void Rotator::Create()
{
	__super::Create();

	canBeMovedInLink = false;

	SetMeshFilename("ico_sphere.vmesh");
}

void Rotator::Start()
{
	__super::Start();

	actorToRotate = dynamic_cast<GridActor*>(World::Get().GetActorByNameAllowNull(actorNameToRotate));
	if (actorToRotate == nullptr)
	{
		Log("[%s] GridActor not found for [%s].", actorNameToRotate.c_str(), GetName().c_str());
	}
}

void Rotator::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (waitOnActorToRotateRotationComplete)
	{
		if (actorToRotate->CheckMovementAndRotationStopped())
		{
			waitOnActorToRotateRotationComplete = false;
			Grid::system.GetOnlyActor()->Awake();
		}
	}
}

Properties Rotator::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Linked Actor", &actorNameToRotate).useActorsAutoComplete = true;
	return props;
}

void Rotator::OnLinkRotate()
{
	__super::OnLinkRotate();

	waitOnActorToRotateRotationComplete = true;
	actorToRotate->SetNextRot(nextRot);
}
