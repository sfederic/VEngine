#include "vpch.h"
#include "Rotator.h"
#include "Core/Log.h"
#include "Components/MeshComponent.h"
#include "Grid.h"

void Rotator::Create()
{
	canBeMovedInLink = false;

	mesh->SetMeshFilename("ico_sphere.vmesh");
}

void Rotator::Start()
{
	__super::Start();

	actorToRotate = dynamic_cast<GridActor*>(World::GetActorByNameAllowNull(actorNameToRotate));
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
		if (actorToRotate->HaveMovementAndRotationStopped())
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
	props.Add("Linked Actor", &actorNameToRotate);
	return props;
}

void Rotator::OnLinkRotate()
{
	waitOnActorToRotateRotationComplete = true;
	actorToRotate->nextRot = nextRot;
}
